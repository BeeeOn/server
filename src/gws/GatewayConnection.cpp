#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>
#include <Poco/Net/NetException.h>

#include "gws/GatewayConnection.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

/// @see https://tools.ietf.org/html/rfc6455#section-5.5
#define CONTROL_PAYLOAD_LIMIT 125
#define FRAME_OP_CONTROL_MASK 0x08

GatewayConnection::GatewayConnection(
		const GatewayID &gatewayID,
		const WebSocket &webSocket,
		SocketReactor &reactor,
		const EnqueueReadable &enqueueReadable,
		size_t maxMessageSize):
	m_gatewayID(gatewayID),
	m_webSocket(webSocket),
	m_reactor(reactor),
	m_enqueueReadable(enqueueReadable),
	m_receiveBuffer(maxMessageSize),
	m_readableObserver(*this, &GatewayConnection::onReadable)
{
	if (logger().debug()) {
		logger().debug("gateway " + m_gatewayID.toString()
				+ " connection created", __FILE__, __LINE__);
	}
}

GatewayConnection::~GatewayConnection()
{
	if (logger().debug()) {
		logger().debug("gateway " + m_gatewayID.toString()
				+ " connection destroyed", __FILE__, __LINE__);
	}
}

GatewayID GatewayConnection::gatewayID() const
{
	return m_gatewayID;
}

void GatewayConnection::addToReactor() const
{
	m_reactor.addEventHandler(m_webSocket, m_readableObserver);
}

void GatewayConnection::removeFromReactor() const
{
	m_reactor.removeEventHandler(m_webSocket, m_readableObserver);
}

void GatewayConnection::updateLastReceiveTime()
{
	FastMutex::ScopedLock guard(m_lastReceiveTimeMutex);
	m_lastReceiveTime.update();
}

Poco::Timestamp GatewayConnection::lastReceiveTime()
{
	FastMutex::ScopedLock guard(m_lastReceiveTimeMutex);
	return m_lastReceiveTime;
}

GWMessage::Ptr GatewayConnection::receiveMessage()
{
	int flags;

	int ret = m_webSocket.receiveFrame(
			m_receiveBuffer.begin(), m_receiveBuffer.size(), flags);

	const int opcode = flags & WebSocket::FRAME_OP_BITMASK;

	if (ret <= 0 || opcode == WebSocket::FRAME_OP_CLOSE)
		throw ConnectionResetException(m_gatewayID.toString());

	if (logger().debug()) {
		logger().debug("received frame with flags: "
			+ NumberFormatter::formatHex(flags, true),
			__FILE__, __LINE__);
	}

	updateLastReceiveTime();

	if (opcode & FRAME_OP_CONTROL_MASK && ret > CONTROL_PAYLOAD_LIMIT)
		throw ProtocolException("too long payload for a control frame");

	string msg(m_receiveBuffer.begin(), ret);

	switch (opcode) {
	case WebSocket::FRAME_OP_TEXT:
		if (!(flags & WebSocket::FRAME_FLAG_FIN))
			throw ProtocolException("multi-fragment messages are unsupported");

		if (logger().debug()) {
			logger().debug("data from gateway "
					+ m_gatewayID.toString() + ":\n" + msg, __FILE__, __LINE__);
		}

		return GWMessage::fromJSON(msg);

	default:
		throw ProtocolException("unhandled websocket opcode: "
				+ NumberFormatter::formatHex(opcode, true));
	}

	return nullptr;
}

void GatewayConnection::sendMessage(const GWMessage::Ptr message)
{
	FastMutex::ScopedLock guard(m_sendMutex);

	const string &msg = message->toString();

	if (logger().debug()) {
		logger().debug("message to gateway "
				+ m_gatewayID.toString() + ":\n" + msg, __FILE__, __LINE__);
	}

	m_webSocket.sendFrame(msg.c_str(), msg.length());
}

void GatewayConnection::onReadable(const AutoPtr<ReadableNotification> &notification)
{
	m_enqueueReadable(AutoPtr<GatewayConnection>(this, true));
}


