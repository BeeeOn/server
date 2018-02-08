#include <Poco/Clock.h>
#include <Poco/Logger.h>
#include <Poco/Net/WebSocket.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "gws/GatewayCommunicator.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GatewayCommunicator)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_REF("messageHandler", &GatewayCommunicator::setGWMessageHandler)
BEEEON_OBJECT_NUMBER("maxMessageSize", &GatewayCommunicator::setMaxMessageSize)
BEEEON_OBJECT_TIME("receiveTimeout", &GatewayCommunicator::setReceiveTimeout)
BEEEON_OBJECT_TIME("sendTimeout", &GatewayCommunicator::setSendTimeout)
BEEEON_OBJECT_NUMBER("minThreads", &GatewayCommunicator::setMinThreads)
BEEEON_OBJECT_NUMBER("maxThreads", &GatewayCommunicator::setMaxThreads)
BEEEON_OBJECT_TIME("threadIdleTime", &GatewayCommunicator::setThreadIdleTime)
BEEEON_OBJECT_REF("asyncExecutor", &GatewayCommunicator::setAsyncExecutor)
BEEEON_OBJECT_REF("listeners", &GatewayCommunicator::registerListener)
BEEEON_OBJECT_END(BeeeOn, GatewayCommunicator)

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

GatewayCommunicator::GatewayCommunicator():
	m_maxMessageSize(4096),
	m_stop(false),
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

void GatewayCommunicator::setAsyncExecutor(SharedPtr<AsyncExecutor> executor)
{
	m_eventSource.setAsyncExecutor(executor);
}

void GatewayCommunicator::addGateway(const GatewayID &gatewayID, WebSocket &webSocket)
{
	FastMutex::ScopedLock guard(m_connectionMapMutex);

	auto it = m_connectionMap.find(gatewayID);
	if (it != m_connectionMap.end()) {
		if (logger().debug()) {
			logger().debug("gateway " + gatewayID.toString()
					+ " found, using new connection", __FILE__, __LINE__);
		}

		it->second->removeFromReactor();
		m_connectionMap.erase(it);
	}

	webSocket.setReceiveTimeout(m_receiveTimeout);
	webSocket.setSendTimeout(m_sendTimeout);

	GatewayConnection::Ptr connection(
		new GatewayConnection(
			gatewayID,
			webSocket,
			m_reactor,
			[this](GatewayConnection::Ptr gatewayConnection)
			{
				enqueueReadable(gatewayConnection);
			},
			m_maxMessageSize
	));

	auto emplaced = m_connectionMap.emplace(gatewayID, connection);
	emplaced.first->second->addToReactor();

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

	it->second->removeFromReactor();
	m_connectionMap.erase(it);

	const GatewayEvent e(id);
	m_eventSource.fireEvent(e, &GatewayListener::onDisconnected);
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

	it->second->removeFromReactor();
	m_connectionMap.erase(it);

	const GatewayEvent e(id);
	m_eventSource.fireEvent(e, &GatewayListener::onDisconnected);
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

	m_stop = false;

	m_reactorThread.start(m_reactor);
}

void GatewayCommunicator::stop()
{
	m_stop = true;

	m_reactor.stop();
	logger().information("waiting to join reactor thread", __FILE__, __LINE__);
	m_reactorThread.join();

	m_connectionReadableQueue.clear();

	logger().information("waiting to join worker threads", __FILE__, __LINE__);
	pool().joinAll();

	FastMutex::ScopedLock guard(m_connectionMapMutex);
	logger().notice("destroying " + to_string(m_connectionMap.size())
			+ " active connections", __FILE__, __LINE__);

	for (auto &it : m_connectionMap) {
		it.second->removeFromReactor();

		const GatewayEvent e(it.second->gatewayID());
		m_eventSource.fireEvent(e, &GatewayListener::onDisconnected);
	}

	m_connectionMap.clear();
}


void GatewayCommunicator::enqueueReadable(GatewayConnection::Ptr connection)
{
	connection->removeFromReactor();

	m_connectionReadableQueue.enqueue(connection);

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

void GatewayCommunicator::runWorker()
{
	while (!m_stop) {
		GatewayConnection::Ptr connection = m_connectionReadableQueue.dequeue();

		if (!connection.isNull())
			handleConnectionReadable(connection);

		// Exit when there is nothing to be done. The thread is returned
		// into the pool.
		if (m_connectionReadableQueue.empty())
			break;
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

	Thread::current()->setName("worker-" + connection->gatewayID().toString());
	const Clock started;

	try {
		GWMessage::Ptr message = connection->receiveMessage();
		connection->addToReactor();

		if (message.isNull())
			return;

		m_messageHandler->handle(message, connection->gatewayID());
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(), removeGateway(connection->gatewayID()));

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
