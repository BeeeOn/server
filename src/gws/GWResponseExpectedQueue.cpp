#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "gws/GWResponseExpectedQueue.h"
#include "util/LambdaTimerTask.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GWResponseExpectedQueue)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_PROPERTY("responseTimeout", &GWResponseExpectedQueue::setResponseTimeout)
BEEEON_OBJECT_PROPERTY("gatewayCommunicator", &GWResponseExpectedQueue::setGatewayCommunicator)
BEEEON_OBJECT_PROPERTY("rpcForwarder", &GWResponseExpectedQueue::setRPCForwarder)
BEEEON_OBJECT_HOOK("cleanup", &GWResponseExpectedQueue::cleanup);
BEEEON_OBJECT_END(BeeeOn, GWResponseExpectedQueue)

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace BeeeOn;

TimerTask::Ptr GWResponseExpectedQueue::createResponseMissingTask(
	const GatewayID &gatewayID, const GlobalID &responseID)
{
	Timestamp taskCreationTime;

	return new LambdaTimerTask([gatewayID, responseID, taskCreationTime, this]()
	{
		if (logger().debug()) {
			logger().debug("response " + responseID.toString()
				+ " from gateway " + gatewayID.toString() + " is missing",
				__FILE__, __LINE__);
		}

		ScopedLockWithUnlock<FastMutex> guard(m_mutex);
		auto it = m_responses.find(make_pair(gatewayID, responseID));
		if (it == m_responses.end())
			return;

		m_responses.erase(it);
		guard.unlock();

		m_rpcForwarder->forwardTimeout(gatewayID, responseID);
		m_gatewayCommunicator->removeIfInactive(gatewayID, taskCreationTime);
	});
}

void GWResponseExpectedQueue::registerResponse(const GatewayID &gatewayID,
	const GlobalID &responseID)
{
	if (logger().debug()) {
		logger().debug("expecting response " + responseID.toString()
			+ " from gateway " + gatewayID.toString(),
			__FILE__, __LINE__);
	}

	TimerTask::Ptr task = createResponseMissingTask(gatewayID, responseID);

	FastMutex::ScopedLock guard(m_mutex);
	m_responses.emplace(make_pair(gatewayID, responseID), task);

	m_timer.schedule(task, Timestamp() + m_responseTimeout);
}

void GWResponseExpectedQueue::notifyDelivered(const GatewayID &gatewayID,
	const GlobalID &responseID)
{
	FastMutex::ScopedLock guard(m_mutex);

	if (logger().debug()) {
		logger().debug("delivered response " + responseID.toString()
					   + " from gateway " + gatewayID.toString(),
			__FILE__, __LINE__);
	}

	auto it = m_responses.find(make_pair(gatewayID, responseID));
	if (it == m_responses.end())
		return;

	it->second->cancel();
	m_responses.erase(it);
}

void GWResponseExpectedQueue::start()
{
	logger().information("response expected queue is up", __FILE__, __LINE__);
}

void GWResponseExpectedQueue::stop()
{
	logger().information("stopping response expected queue", __FILE__, __LINE__);

	m_timer.cancel(true);

	FastMutex::ScopedLock guard(m_mutex);
	m_responses.clear();

	logger().information("response expected queue has stopped", __FILE__, __LINE__);
}

void GWResponseExpectedQueue::cleanup()
{
	m_gatewayCommunicator = nullptr;
	m_rpcForwarder = nullptr;
}

void GWResponseExpectedQueue::setResponseTimeout(const Timespan &timeout)
{
	if (timeout < 0)
		throw InvalidArgumentException("response timeout must be non negative");

	m_responseTimeout = timeout;
}

void GWResponseExpectedQueue::setGatewayCommunicator(
		GatewayCommunicator::Ptr communicator)
{
	m_gatewayCommunicator = communicator;
}

void GWResponseExpectedQueue::setRPCForwarder(RPCForwarder::Ptr forwarder)
{
	m_rpcForwarder = forwarder;
}
