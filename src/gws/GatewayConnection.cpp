#include <Poco/Logger.h>
#include <Poco/NumberFormatter.h>
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
		GatewayRateLimiter::Ptr rateLimiter,
		const EnqueueReadable &enqueueReadable,
		size_t maxMessageSize):
	WebSocketConnection(
		gatewayID.toString(),
		webSocket,
		reactor,
		[enqueueReadable](WebSocketConnection::Ptr c) {
			enqueueReadable(c.cast<GatewayConnection>());
		},
		maxMessageSize),
	m_gatewayID(gatewayID),
	m_rateLimiter(rateLimiter)
{
	afterConstructed();
}

GatewayConnection::~GatewayConnection()
{
}

GatewayID GatewayConnection::gatewayID() const
{
	return m_gatewayID;
}

GatewayRateLimiter::Ptr GatewayConnection::rateLimiter() const
{
	return m_rateLimiter;
}

void GatewayConnection::updateLastReceiveTime()
{
	FastMutex::ScopedLock guard(m_lastReceiveTimeMutex);
	m_lastReceiveTime.update();
}

Poco::Timestamp GatewayConnection::lastReceiveTime() const
{
	FastMutex::ScopedLock guard(m_lastReceiveTimeMutex);
	return m_lastReceiveTime;
}

GWMessage::Ptr GatewayConnection::filterMessage(const std::string &data)
{
	GWMessage::Ptr msg = GWMessage::fromJSON(data);

	if (!m_rateLimiter->accept(msg)) {
		if (logger().debug()) {
			logger().debug("rate limiting gateway "
				+ gatewayID().toString(),
				__FILE__, __LINE__);
		}

		m_stats.lost(data.size());
		return nullptr;
	}

	return msg;
}

GWMessage::Ptr GatewayConnection::receiveMessage()
{
	Nullable<string> message;

	try {
		message = receiveFrame();
	}
	catch (const ProtocolException &e) {
		updateLastReceiveTime();
		e.rethrow();
	}

	updateLastReceiveTime();

	if (message.isNull())
		return nullptr;

	return filterMessage(message);
}

void GatewayConnection::handlePing(const Buffer<char> &request, size_t length)
{
	if (!m_rateLimiter->accept()) {
		if (logger().debug()) {
			logger().debug("rate limiting ping-pong for "
				+ gatewayID().toString(),
				__FILE__, __LINE__);
		}

		m_stats.lost(length);
		return;
	}

	sendPong(request, length);
}

void GatewayConnection::sendMessage(const GWMessage::Ptr message)
{
	const auto &msg = message->toString();
	sendFrame(msg, msg.size());
}
