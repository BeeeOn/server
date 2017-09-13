#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>

#include "gws/GatewayConnection.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

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
				+ " connection created");
	}
}

GatewayConnection::~GatewayConnection()
{
	if (logger().debug()) {
		logger().debug("gateway " + m_gatewayID.toString()
				+ " connection destroyed");
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

	if (ret <= 0 || (flags & WebSocket::FRAME_OP_CLOSE))
		throw ConnectionResetException(m_gatewayID.toString());

	updateLastReceiveTime();

	string msg(m_receiveBuffer.begin(), ret);

	if (logger().debug()) {
		logger().debug("data from gateway "
				+ m_gatewayID.toString() + ":\n" + msg);
	}

	return GWMessage::fromJSON(msg);
}

void GatewayConnection::sendMessage(const GWMessage::Ptr message)
{
	FastMutex::ScopedLock guard(m_sendMutex);

	const string &msg = message->toString();

	if (logger().debug()) {
		logger().debug("message to gateway "
				+ m_gatewayID.toString() + ":\n" + msg);
	}

	m_webSocket.sendFrame(msg.c_str(), msg.length());
}

void GatewayConnection::onReadable(const AutoPtr<ReadableNotification> &notification)
{
	m_enqueueReadable(AutoPtr<GatewayConnection>(this, true));
}


