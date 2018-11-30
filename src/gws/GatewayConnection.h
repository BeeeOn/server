#pragma once

#include <functional>

#include <Poco/AutoPtr.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>

#include "gwmessage/GWMessage.h"
#include "model/GatewayID.h"
#include "policy/GatewayRateLimiter.h"
#include "server/WebSocketConnection.h"

namespace BeeeOn {

/**
 * @brief Represents a connection with the specific gateway.
 *
 * The GatewayConnection is created and stored in the GatewayCommunicator.
 * It is identified by the identifier of the connected gateway. Each connection
 * is managed by the Poco::Net::SocketReactor. The reactor notifies the system
 * about available readable connections.
 *
 * Once the connection is created it is registered as an event handler
 * to the reactor. When the reactor detects that the connection is readable,
 * the connection is unregistered from the reactor, it is queued and worker
 * threads from the thread pool of the GatewayCommunicator read GWMessage
 * from it. The worker then register the connection back to the reactor
 * and handle received message.
 *
 * Other parts of the system can access the GatewayConnection through the
 * GatewayCommunicator and send GWMessage to the gateway.
 *
 * The connection is destroyed, if:
 * - socket is closed or broken
 * - received message is invalid (unknown message type, missing field, etc.)
 * - new connection with the same id (socket incorrectly closed and gateway
 *   reconnected)
 * - connection is considered inactive (the response to the request has not
 *   arrived within the timeout and in the meantime no other message came
 *   from the gateway.)
 */
class GatewayConnection : public WebSocketConnection {
public:
	typedef Poco::AutoPtr<GatewayConnection> Ptr;
	typedef std::function<void(GatewayConnection::Ptr)> EnqueueReadable;

	GatewayConnection(
		const GatewayID &gatewayID,
		const Poco::Net::WebSocket &webSocket,
		GatewayRateLimiter::Ptr rateLimiter,
		size_t maxMessageSize
	);

	~GatewayConnection();

	/**
	 * @brief Returns id of the connected gateway.
	 */
	GatewayID gatewayID() const;

	/**
	 * @brief rate limiter of the connection.
	 */
	GatewayRateLimiter::Ptr rateLimiter() const;

	/**
	 * @brief Returns timestamp of the last received message.
	 */
	Poco::Timestamp lastReceiveTime() const;

	/**
	 * @brief Sends GWMessage to the gateway. Method is thread-safe.
	 */
	void sendMessage(const GWMessage::Ptr message);

	/**
	 * @brief Receives a GWMessage and returns it. Method is not thread-safe.
	 */
	GWMessage::Ptr receiveMessage();

protected:
	/**
	 * @brief Filter pings by the GatewayRateLimiter.
	 */
	void handlePing(const Poco::Buffer<char> &request, size_t length) override;

private:
	GWMessage::Ptr filterMessage(const std::string &data);

	/**
	 * @brief Set timestamp of the last received message.
	 */
	void updateLastReceiveTime();

private:
	GatewayID m_gatewayID;
	GatewayRateLimiter::Ptr m_rateLimiter;
	EnqueueReadable m_enqueueReadable;
	Poco::Timestamp m_lastReceiveTime;
	mutable Poco::FastMutex m_lastReceiveTimeMutex;
};

}
