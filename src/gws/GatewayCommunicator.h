#pragma once

#include <deque>
#include <functional>

#include <Poco/Mutex.h>
#include <Poco/NotificationQueue.h>
#include <Poco/Nullable.h>
#include <Poco/RunnableAdapter.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/SocketReactor.h>

#include "gwmessage/GWMessage.h"
#include "gws/GatewayConnection.h"
#include "gws/GatewayListener.h"
#include "gws/GWMessageHandler.h"
#include "loop/StoppableLoop.h"
#include "loop/StopControl.h"
#include "model/GatewayID.h"
#include "policy/GatewayRateLimiter.h"
#include "util/EventSource.h"
#include "util/HavingThreadPool.h"
#include "util/Occasionally.h"

namespace BeeeOn {

/**
 * @brief GatewayCommunicator is responsible for communication with gateways
 * connected to the server.
 *
 * It maintains a table of {GatewayID, GatewayConnection::Ptr} pairs.
 * Each connection is managed by the Poco::SocketReactor that is running
 * in a separate thread. The reactor notifies the system about available
 * readable connections.
 *
 * The ConnectionReadableQueue is used for queueing of readable connections to be
 * handled by threads allocated from a thread pool. After a connection is marked
 * as readable (via SocketReactor), it is unregistered from the reactor
 * and it is queued in the notification queue. When there are some queued
 * connections, a worker threads are allocated from the associated
 * thread pool to handle all of them as soon as possible.
 *
 * When a message is read from a gateway connection, the connection is again
 * registered into the reactor to wait for more data,
 * and the GWMessageHandler::handle(GatewayMessage::Ptr) is called to do the actual
 * message processing. Any unhandled exception during handling message causes
 * deleting the gateway connection.
 *
 * Handling of gateway connections is fair. This is accomplished by using
 * the Poco::SocketReactor that watches the open socket connections for changes.
 */
class GatewayCommunicator :
	public StoppableLoop,
	public HavingThreadPool {
public:
	/**
	 * @brief The ConnectionReadableQueue is intended for queueing of readable
	 * connections to be handled by worker threads of the GatewayCommunicator.
	 */
	class ConnectionReadableQueue {
	public:
		~ConnectionReadableQueue();

		/**
		 * @brief Enqueues the given connection by adding it to the end
		 * of the queue (FIFO).
		 */
		void enqueue(GatewayConnection::Ptr connection);

		/**
		 * @brief Dequeues the next pending connection.
		 *
		 * Returns nullptr if no connection is available.
		 */
		GatewayConnection::Ptr dequeue();

		/**
		 * @brief Returns true if the queue is empty.
		 */
		bool empty() const;

		/**
		 * @brief Returns the number of connections in the queue.
		 */
		unsigned long size() const;

		/**
		 * @brief Removes all connections from the queue.
		 */
		void clear();

	private:
		std::deque<GatewayConnection::Ptr> m_queue;
		mutable Poco::FastMutex m_mutex;
	};

	typedef Poco::SharedPtr<GatewayCommunicator> Ptr;

	GatewayCommunicator();
	~GatewayCommunicator();

	 /**
	  * @brief Create new GatewayConnection for the given id and add it
	  * to the communicator.
	  *
	  * If there is a GatewayConnection with the given id
	  * in the GatewayCommunicator, it is replaced by new one.
	  */
	void addGateway(const GatewayID &gatewayID, Poco::Net::WebSocket &webSocket);

	/**
	 * @brief Remove Gateway from the GatewayCommunicator.
	 *
	 * If there is no a GatewayConnection with the given id, it is ignored.
	 */
	void removeGateway(const GatewayID &gatewayID);

	/**
	 * @brief Remove Gateway from the GatewayCommunicator,
	 * if no message was received from the given time till now.
	 *
	 * If there is no a GatewayConnection with the given id, it is ignored.
	 */
	void removeIfInactive(const GatewayID &id, const Poco::Timestamp &timeFrom);

	/**
	 * @returns timestamp of the most recent activity of the given gateway
	 * or null if no such gateway is connected
	 */
	Poco::Nullable<Poco::Timestamp> lastActivity(const GatewayID &id) const;

	/**
	 * @brief Sends GWMessage to the connected gateway.
	 *
	 * @throw NotFoundException if GatewayCommunicator
	 * has no such GatewayConnection.
	 */
	void sendMessage(const GatewayID &gatewayID, const GWMessage::Ptr message);

	void setGWMessageHandler(GWMessageHandler::Ptr handler);
	void setRateLimiterFactory(GatewayRateLimiterFactory::Ptr factory);

	void setMaxMessageSize(int size);
	void setReceiveTimeout(const Poco::Timespan &timeout);
	void setSendTimeout(const Poco::Timespan &timeout);

	void registerListener(GatewayListener::Ptr listener);
	void setEventsExecutor(AsyncExecutor::Ptr executor);

	void start() override;
	void stop() override;

private:
	/**
	 * @brief Returns GatewayConnection with the given id.
	 *
	 * @throw NotFoundException if GatewayCommunicator
	 * has no such GatewayConnection.
	 */
	GatewayConnection::Ptr findConnection(const GatewayID &gatewayID);

	/**
	 * @brief Enqueue given GatewayConnection to the readable queue
	 * and start worker in the thread pool.
	 *
	 * The GatewayConnection must be unregistered from the reactor.
	 *
	 * If there is no available thread in the thread pool, connection is
	 * just queued and other running worker handles it.
	 */
	void enqueueReadable(GatewayConnection::Ptr connection);

	/**
	 * @brief Receives a GWMessage from the GatewayConnection and handle it
	 * using GWMessageHandler.
	 *
	 * Any exception thrown from the GWConnection receiveMessage() method
	 * and also from the GWMessageHandler handle() method causes removing
	 * the connection from the GatewayCommunicator.
	 */
	void handleConnectionReadable(GatewayConnection::Ptr connection);

	/**
	 * @brief The worker dequeues readable connection and handle it.
	 *
	 * The worker is started in the thread pool after enqueueing readable
	 * connection to the queue. It dequeues readable connection and handles it.
	 * If the queue is not empty, worker continues and handles the next
	 * readable connection, otherwise it will end.
	 */
	void runWorker();

	/**
	 * @brief Remove the connection from reactor and fire event
	 * on-disconnected for the associated gateway.
	 */
	void closeConnection(GatewayConnection::Ptr connection);

private:
	typedef std::map<GatewayID, GatewayConnection::Ptr> GatewayConnectionMap;

	size_t m_maxMessageSize;
	Poco::Timespan m_receiveTimeout;
	Poco::Timespan m_sendTimeout;

	GWMessageHandler::Ptr m_messageHandler;
	GatewayRateLimiterFactory::Ptr m_rateLimiterFactory;

	GatewayConnectionMap m_connectionMap;
	mutable Poco::FastMutex m_connectionMapMutex;
	ConnectionReadableQueue m_connectionReadableQueue;

	Poco::Net::SocketReactor m_reactor;
	Poco::Thread m_reactorThread;
	StopControl m_stopControl;
	Poco::RunnableAdapter<GatewayCommunicator> m_workerRunnable;

	EventSource<GatewayListener> m_eventSource;
};

}
