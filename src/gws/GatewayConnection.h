#ifndef BEEEON_GATEWAY_CONNECTION_H
#define BEEEON_GATEWAY_CONNECTION_H

#include <functional>

#include <Poco/AutoPtr.h>
#include <Poco/Buffer.h>
#include <Poco/NObserver.h>
#include <Poco/RefCountedObject.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>
#include <Poco/Net/SocketNotification.h>
#include <Poco/Net/SocketReactor.h>
#include <Poco/Net/WebSocket.h>

#include "gwmessage/GWMessage.h"
#include "model/GatewayID.h"
#include "policy/GatewayRateLimiter.h"
#include "util/Loggable.h"

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
class GatewayConnection :
		public Poco::RefCountedObject,
		public Loggable {
public:
	typedef Poco::AutoPtr<GatewayConnection> Ptr;
	typedef std::function<void(GatewayConnection::Ptr)> EnqueueReadable;

	GatewayConnection(
		const GatewayID &gatewayID,
		const Poco::Net::WebSocket &webSocket,
		Poco::Net::SocketReactor &reactor,
		GatewayRateLimiter::Ptr rateLimiter,
		const EnqueueReadable &enqueueReadable,
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
	 * @brief Register GatewayConnection as an event handler to the SocketReactor.
	 */
	void addToReactor() const;

	/**
	 * @brief Unregister GatewayConnection as an event handler from the SocketReactor.
	 */
	void removeFromReactor() const;

	/**
	 * @brief Returns timestamp of the last received message.
	 */
	Poco::Timestamp lastReceiveTime();

	/**
	 * @brief Sends GWMessage to the gateway. Method is thread-safe.
	 */
	void sendMessage(const GWMessage::Ptr message);

	/**
	 * @brief Receives a GWMessage and returns it. Method is not thread-safe.
	 */
	GWMessage::Ptr receiveMessage();

private:
	GWMessage::Ptr filterMessage(GWMessage::Ptr message);

	/**
	 * @brief Sends back the PONG frame. Such frame must contain the same
	 * application data as it was received.
	 *
	 * @see https://tools.ietf.org/html/rfc6455#section-5.5.2
	 */
	void sendPong(const std::string &requestData);

	/**
	 * @brief Set timestamp of the last received message.
	 */
	void updateLastReceiveTime();

	/**
	 * @brief The Callback method invoked from the reactor thread.
	 *
	 * This method is intended just to determine there are data on the socket.
	 * The data reading takes place in the thread pool of the GatewayCommunicator.
	 */
	void onReadable(const Poco::AutoPtr<Poco::Net::ReadableNotification> &notification);

private:
	GatewayID m_gatewayID;
	Poco::Net::WebSocket m_webSocket;
	Poco::Net::SocketReactor &m_reactor;
	GatewayRateLimiter::Ptr m_rateLimiter;
	EnqueueReadable m_enqueueReadable;
	Poco::Buffer<char> m_receiveBuffer;
	Poco::Timestamp m_lastReceiveTime;
	Poco::FastMutex m_lastReceiveTimeMutex;
	Poco::FastMutex m_sendMutex;
	Poco::NObserver<GatewayConnection, Poco::Net::ReadableNotification> m_readableObserver;
};

}
#endif
