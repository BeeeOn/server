#include <Poco/Logger.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "gws/GatewayCommunicator.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GatewayCommunicator)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("messageHandler", &GatewayCommunicator::setGWMessageHandler)
BEEEON_OBJECT_PROPERTY("rateLimiterFactory", &GatewayCommunicator::setRateLimiterFactory)
BEEEON_OBJECT_PROPERTY("maxMessageSize", &GatewayCommunicator::setMaxMessageSize)
BEEEON_OBJECT_PROPERTY("receiveTimeout", &GatewayCommunicator::setReceiveTimeout)
BEEEON_OBJECT_PROPERTY("sendTimeout", &GatewayCommunicator::setSendTimeout)
BEEEON_OBJECT_PROPERTY("maxBulkDuration", &GatewayCommunicator::setMaxBulkDuration)
BEEEON_OBJECT_PROPERTY("maxBulkSize", &GatewayCommunicator::setMaxBulkSize)
BEEEON_OBJECT_PROPERTY("minThreads", &GatewayCommunicator::setMinThreads)
BEEEON_OBJECT_PROPERTY("maxThreads", &GatewayCommunicator::setMaxThreads)
BEEEON_OBJECT_PROPERTY("threadIdleTime", &GatewayCommunicator::setThreadIdleTime)
BEEEON_OBJECT_PROPERTY("eventsExecutor", &GatewayCommunicator::setEventsExecutor)
BEEEON_OBJECT_PROPERTY("listeners", &GatewayCommunicator::registerListener)
BEEEON_OBJECT_END(BeeeOn, GatewayCommunicator)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

GatewayCommunicator::GatewayCommunicator():
	m_maxMessageSize(4096),
	m_maxBulkDuration(1 * Timespan::SECONDS),
	m_maxBulkSize(16),
	m_readableObserver(*this, &GatewayCommunicator::onReadable),
	m_workerRunnable(*this, &GatewayCommunicator::runWorker)
{
}

GatewayCommunicator::~GatewayCommunicator()
{
}

void GatewayCommunicator::registerListener(GatewayListener::Ptr listener)
{
	m_eventSource.addListener(listener);
}

void GatewayCommunicator::setEventsExecutor(AsyncExecutor::Ptr executor)
{
	m_eventSource.setAsyncExecutor(executor);
}

void GatewayCommunicator::addGateway(const GatewayID &gatewayID, WebSocket &webSocket)
{
	FastMutex::ScopedLock guard(m_connectionMapMutex);

	GatewayRateLimiter::Ptr rateLimiter;

	auto it = m_connectionMap.find(gatewayID);
	if (it != m_connectionMap.end()) {
		if (logger().debug()) {
			logger().debug("gateway " + gatewayID.toString()
					+ " found, using new connection", __FILE__, __LINE__);
		}

		rateLimiter = it->second->rateLimiter();
		m_reactor.removeEventHandler(it->second->socket(), m_readableObserver);
		m_socketsMap.erase(it->second->socket());
		m_connectionMap.erase(it);
	}
	else {
		rateLimiter = m_rateLimiterCache.findAndDrop(gatewayID);
	}

	if (rateLimiter.isNull())
		rateLimiter = m_rateLimiterFactory->create(gatewayID);

	webSocket.setReceiveTimeout(m_receiveTimeout);
	webSocket.setSendTimeout(m_sendTimeout);

	GatewayConnection::Ptr connection(
		new GatewayConnection(
			gatewayID,
			webSocket,
			rateLimiter,
			m_maxMessageSize
	));

	auto emplaced = m_connectionMap.emplace(gatewayID, connection);
	poco_assert(emplaced.second);
	m_socketsMap.emplace(webSocket, connection);
	m_reactor.addEventHandler(webSocket, m_readableObserver);

	GatewayEvent e(gatewayID);
	e.setSocketAddress(webSocket.peerAddress());

	if (it != m_connectionMap.end())
		m_eventSource.fireEvent(e, &GatewayListener::onReconnected);
	else
		m_eventSource.fireEvent(e, &GatewayListener::onConnected);
}

void GatewayCommunicator::removeGateway(const GatewayID &id)
{
	FastMutex::ScopedLock guard(m_connectionMapMutex);

	auto it = m_connectionMap.find(id);
	if (it == m_connectionMap.end()) {
		logger().warning("gateway " + id.toString() + " not found",
			__FILE__, __LINE__);
		return;
	}

	closeConnection(it->second);
	m_socketsMap.erase(it->second->socket());
	m_connectionMap.erase(it);
}

void GatewayCommunicator::removeIfInactive(const GatewayID &id,
	const Timestamp &timeFrom)
{
	FastMutex::ScopedLock guard(m_connectionMapMutex);

	auto it = m_connectionMap.find(id);
	if (it == m_connectionMap.end()) {
		logger().warning("gateway " + id.toString() + " not found",
			__FILE__, __LINE__);
		return;
	}

	if (timeFrom < it->second->lastReceiveTime())
		return;

	closeConnection(it->second);
	m_socketsMap.erase(it->second->socket());
	m_connectionMap.erase(it);
}

Nullable<Timestamp> GatewayCommunicator::lastActivity(const GatewayID &id) const
{
	FastMutex::ScopedLock guard(m_connectionMapMutex);

	auto it = m_connectionMap.find(id);
	if (it == m_connectionMap.end())
		return {};

	return it->second->lastReceiveTime();
}

GatewayConnection::Ptr GatewayCommunicator::findConnection(const GatewayID &id)
{
	FastMutex::ScopedLock guard(m_connectionMapMutex);

	auto it = m_connectionMap.find(id);
	if (it == m_connectionMap.end())
		throw NotFoundException("gateway: " + id.toString());

	return it->second;
}

void GatewayCommunicator::sendMessage(const GatewayID &gatewayID,
	const GWMessage::Ptr message)
{
	findConnection(gatewayID)->sendMessage(message);
}

void GatewayCommunicator::start()
{
	initPool();
	m_reactorThread.start(m_reactor);
}

void GatewayCommunicator::stop()
{
	m_stopControl.requestStop();

	m_reactor.stop();
	logger().information("waiting to join reactor thread", __FILE__, __LINE__);
	m_reactorThread.join();

	m_connectionReadableQueue.clear();

	logger().information("waiting to join worker threads", __FILE__, __LINE__);
	pool().joinAll();

	FastMutex::ScopedLock guard(m_connectionMapMutex);
	logger().notice("destroying " + to_string(m_connectionMap.size())
			+ " active connections", __FILE__, __LINE__);

	for (auto &it : m_connectionMap)
		closeConnection(it.second);

	m_connectionMap.clear();
	m_socketsMap.clear();
}

void GatewayCommunicator::closeConnection(GatewayConnection::Ptr connection)
{
	m_reactor.removeEventHandler(connection->socket(), m_readableObserver);
	m_rateLimiterCache.keep(connection->rateLimiter());

	const GatewayEvent e(connection->gatewayID());
	m_eventSource.fireEvent(e, &GatewayListener::onDisconnected);
}

void GatewayCommunicator::runWorker()
{
	StopControl::Run run(m_stopControl);

	while (run) {
		GatewayConnection::Ptr connection = m_connectionReadableQueue.dequeue();

		if (!connection.isNull())
			handleConnectionReadable(connection);

		// Exit when there is nothing to be done. The thread is returned
		// into the pool.
		if (m_connectionReadableQueue.empty())
			break;
	}
}

void GatewayCommunicator::onReadable(const AutoPtr<ReadableNotification> &notification)
{
	FastMutex::ScopedLock guard(m_connectionMapMutex);

	WebSocket socket(notification->socket());

	auto it = m_socketsMap.find(socket);
	if (it == m_socketsMap.end()) {
		if (logger().debug()) {
			logger().debug(
				"handling incoming data for just disconnected gateway at "
				+ socket.peerAddress().toString());
		}

		m_reactor.removeEventHandler(socket, m_readableObserver);
		return;
	}

	m_reactor.removeEventHandler(socket, m_readableObserver);
	m_connectionReadableQueue.enqueue(it->second);

	try {
		pool().start(m_workerRunnable);
	}
	catch (const NoThreadAvailableException &e) {
		logger().warning(e.displayText(), __FILE__, __LINE__);
		// All configured threads are busy and thus there is no need to start
		// threads anymore. The connection is already queued and thus some
		// thread would serve it soon.
	}
}

void GatewayCommunicator::handleConnectionReadable(GatewayConnection::Ptr connection)
{
	static Occasionally occasionally;

	occasionally.execute([this]() {
		ScopedLockWithUnlock<FastMutex> guard(m_connectionMapMutex);
		int connections = m_connectionMap.size();
		guard.unlock();

		logger().notice(
			"active connections: " + to_string(connections)
			+ ", readable queue: " + to_string(m_connectionReadableQueue.size())
			+ ", thread pool allocated: " + to_string(pool().allocated())
			+ ", available: " + to_string(pool().available())
			+ ", capacity: " + to_string(pool().capacity()),
			__FILE__, __LINE__);
	});

	Thread::current()->setName("gws-worker-" + connection->gatewayID().toString());

	const Clock started;
	size_t i = 0;

	try {
		while (handleNext(started, i)) {
			handleMessage(connection);
			i += 1;
		}

		m_reactor.addEventHandler(connection->socket(), m_readableObserver);
	}
	catch (const TimeoutException &) {
		m_reactor.addEventHandler(connection->socket(), m_readableObserver);
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		removeGateway(connection->gatewayID()))

	if (logger().information() && i > 0) {
		logger().notice("bulk (" + to_string(i)
			+ ") duration: "
			+ to_string(started.elapsed()) + "us",
			__FILE__, __LINE__);
	}
}

bool GatewayCommunicator::handleNext(const Clock &started, size_t i) const
{
	if (m_maxBulkDuration > 0 && m_maxBulkDuration <= started.elapsed())
		return false;

	if (m_maxBulkSize > 0 && m_maxBulkSize <= i)
		return false;

	return true;
}

void GatewayCommunicator::handleMessage(GatewayConnection::Ptr connection)
{
	const Clock started;

	GWMessage::Ptr message = connection->receiveMessage();

	if (!message.isNull())
		m_messageHandler->handle(message, connection->gatewayID());

	if (logger().information()) {
		logger().information("duration: "
			+ to_string(started.elapsed()) + "us",
			__FILE__, __LINE__);
	}
}

void GatewayCommunicator::setGWMessageHandler(GWMessageHandler::Ptr handler)
{
	m_messageHandler = handler;
}

void GatewayCommunicator::setRateLimiterFactory(GatewayRateLimiterFactory::Ptr factory)
{
	m_rateLimiterFactory = factory;
}

void GatewayCommunicator::setMaxMessageSize(int size)
{
	if (size < 0)
		throw InvalidArgumentException("size must be non negative");

	m_maxMessageSize = size;
}

void GatewayCommunicator::setReceiveTimeout(const Poco::Timespan &timeout)
{
	if (timeout < 0)
		throw InvalidArgumentException("receive timeout must be non negative");

	m_receiveTimeout = timeout;
}

void GatewayCommunicator::setSendTimeout(const Poco::Timespan &timeout)
{
	if (timeout < 0)
		throw InvalidArgumentException("send timeout must be non negative");

	m_sendTimeout = timeout;
}

void GatewayCommunicator::setMaxBulkDuration(const Timespan &duration)
{
	if (duration < 0)
		throw InvalidArgumentException("maxBulkDuration must not be negative");

	m_maxBulkDuration = duration;
}

void GatewayCommunicator::setMaxBulkSize(int size)
{
	if (size < 0)
		throw InvalidArgumentException("maxBulkSize must not be negative");

	m_maxBulkSize = size;
}

GatewayCommunicator::ConnectionReadableQueue::~ConnectionReadableQueue()
{
	clear();
}

void GatewayCommunicator::ConnectionReadableQueue::enqueue(
	GatewayConnection::Ptr connection)
{
	FastMutex::ScopedLock guard(m_mutex);
	m_queue.push_back(connection);
}

GatewayConnection::Ptr GatewayCommunicator::ConnectionReadableQueue::dequeue()
{
	GatewayConnection::Ptr connection;

	FastMutex::ScopedLock guard(m_mutex);
	if (!m_queue.empty()) {
		connection = m_queue.front();
		m_queue.pop_front();
	}
	return connection;
}

bool GatewayCommunicator::ConnectionReadableQueue::empty() const
{
	FastMutex::ScopedLock guard(m_mutex);
	return m_queue.empty();
}

unsigned long GatewayCommunicator::ConnectionReadableQueue::size() const
{
	FastMutex::ScopedLock guard(m_mutex);
	return m_queue.size();
}

void GatewayCommunicator::ConnectionReadableQueue::clear()
{
	FastMutex::ScopedLock guard(m_mutex);
	m_queue.clear();
}
