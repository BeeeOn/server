#include "di/Injectable.h"
#include "gwmessage/GWListenRequest.h"
#include "gwmessage/GWUnpairRequest.h"
#include "gwmessage/GWPingRequest.h"
#include "gwmessage/GWSetValueRequest.h"
#include "rpc/AsyncGatewayRPC.h"

BEEEON_OBJECT_BEGIN(BeeeOn, AsyncGatewayRPC)
BEEEON_OBJECT_CASTABLE(GatewayRPC)
BEEEON_OBJECT_CASTABLE(RPCForwarder)
BEEEON_OBJECT_CASTABLE(StoppableLoop)
BEEEON_OBJECT_REF("gatewayCommunicator", &AsyncGatewayRPC::setGatewayCommunicator)
BEEEON_OBJECT_REF("responseExpectedQueue", &AsyncGatewayRPC::setGWResponseExpectedQueue)
BEEEON_OBJECT_TIME("finalResultTimeout", &AsyncGatewayRPC::setFinalResultTimeout)
BEEEON_OBJECT_END(BeeeOn, AsyncGatewayRPC)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

void AsyncGatewayRPC::sendListen(const ResultCall &resultCall,
		const Gateway &gateway,
		const Timespan &duration)
{
	CallID callID = CallID::random();

	GWListenRequest::Ptr request = new GWListenRequest;
	request->setID(callID);
	request->setDuration(duration);

	sendAndExpectResult(gateway.id(), callID, resultCall, request);
}

void AsyncGatewayRPC::unpairDevice(const ResultCall &resultCall,
		const Gateway &gateway,
		const Device &device)
{
	CallID callID = CallID::random();

	GWUnpairRequest::Ptr request = new GWUnpairRequest;
	request->setID(callID);
	request->setDeviceID(device.id());

	sendAndExpectResult(gateway.id(), callID, resultCall, request);
}

void AsyncGatewayRPC::pingGateway(const ResultCall &resultCall,
		const Gateway &gateway)
{
	CallID callID = CallID::random();

	GWPingRequest::Ptr request = new GWPingRequest;
	request->setID(callID);

	sendAndExpectResult(gateway.id(), callID, resultCall, request);
}

void AsyncGatewayRPC::updateActor(const ResultCall &resultCall,
		const Gateway &gateway,
		const Device &device,
		const ModuleInfo &module,
		double value,
		const Timespan &timeout)
{
	CallID callID = CallID::random();

	GWSetValueRequest::Ptr request = new GWSetValueRequest;
	request->setID(callID);
	request->setDeviceID(device.id());
	request->setModuleID((unsigned int) module.id());
	request->setValue(value);
	request->setTimeout(timeout);

	sendAndExpectResult(gateway.id(), callID, resultCall, request);
}

LambdaTimerTask::Ptr AsyncGatewayRPC::createFinalResultMissingTask(
		const GatewayID &gatewayID, const CallID &callID)
{
	return new LambdaTimerTask([gatewayID, callID, this]()
	{
		logger().warning("result for call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString()
			+ " is missing",
			__FILE__, __LINE__);

		ScopedLockWithUnlock<FastMutex> guard(m_mutex);

		auto it = m_resultCalls.find(make_pair(gatewayID, callID));
		if (it == m_resultCalls.end())
			return;

		const ResultCall &resultCall = it->second.resultCall;

		m_resultCalls.erase(it);
		guard.unlock();

		GatewayRPCResult::Ptr result = new GatewayRPCResult();
		result->setStatus(GatewayRPCResult::Status::TIMEOUT);

		executeResultCall(gatewayID, callID, resultCall, result);
	});
}

void AsyncGatewayRPC::storeResultCall(const GatewayID &gatewayID,
		const CallID &callID,
		const ResultCall &resultCall)
{
	ResultCallContext context {
		resultCall,
		createFinalResultMissingTask(gatewayID, callID)
	};

	FastMutex::ScopedLock guard(m_mutex);

	auto it = m_resultCalls.emplace(make_pair(gatewayID, callID), context);
	if (!it.second) {
		throw IllegalStateException("duplicate result call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString());
	}

	m_timer.schedule(
		context.finalResultMissingTask, Timestamp() + m_finalResultTimeout);
}

void AsyncGatewayRPC::removeResultCall(const GatewayID &gatewayID,
		const CallID &callID)
{
	FastMutex::ScopedLock guard(m_mutex);

	auto it = m_resultCalls.find(make_pair(gatewayID, callID));
	if (it == m_resultCalls.end()) {
		logger().warning("result call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString()
			+ " was not found",
			__FILE__, __LINE__);

		return;
	}

	it->second.finalResultMissingTask->cancel();
	m_resultCalls.erase(it);
}

void AsyncGatewayRPC::executeResultCall(const GatewayID &gatewayID,
	const AsyncGatewayRPC::CallID &callID,
	const ResultCall &resultCall,
	GatewayRPCResult::Ptr result)
{
	if (logger().debug()) {
		logger().debug("executing result call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString()
			+ " with result "
			+ to_string(result->status()),
			__FILE__, __LINE__);
	}

	try {
		resultCall(result);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const exception &e) {
		logger().critical(e.what(), __FILE__, __LINE__);
	}
	catch (...) {
		logger().critical("unknown error", __FILE__, __LINE__);
	}
}

void AsyncGatewayRPC::sendAndExpectResult(const GatewayID &gatewayID,
		const AsyncGatewayRPC::CallID &callID,
		const GatewayRPC::ResultCall &resultCall,
		const GWRequest::Ptr request)
{
	GatewayRPCResult::Ptr result = new GatewayRPCResult;

	storeResultCall(gatewayID, callID, resultCall);
	m_responseExpectedQueue->registerResponse(gatewayID, callID);

	try {
		m_gatewayCommunicator->sendMessage(gatewayID, request);
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);

		removeResultCall(gatewayID, callID);
		m_responseExpectedQueue->notifyDelivered(gatewayID, callID);

		result->setStatus(GatewayRPCResult::Status::NOT_CONNECTED);
		executeResultCall(gatewayID, callID, resultCall, result);
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);

		removeResultCall(gatewayID, callID);
		m_responseExpectedQueue->notifyDelivered(gatewayID, callID);

		result->setStatus(GatewayRPCResult::Status::FAILED);
		executeResultCall(gatewayID, callID, resultCall, result);
	}
}

void AsyncGatewayRPC::processResult(const GatewayID &gatewayID,
		const CallID &callID,
		GatewayRPCResult::Ptr result)
{
	ScopedLockWithUnlock<FastMutex> guard(m_mutex);

	auto it = m_resultCalls.find(make_pair(gatewayID, callID));
	if (it == m_resultCalls.end()) {
		logger().warning("result call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString()
			+ " was not found",
			__FILE__, __LINE__);

		return;
	}

	const ResultCall &resultCall = it->second.resultCall;

	switch (result->status()) {
	case GatewayRPCResult::Status::ACCEPTED:
		break;
	case GatewayRPCResult::Status::TIMEOUT:
	case GatewayRPCResult::Status::SUCCESS:
	case GatewayRPCResult::Status::FAILED:
		it->second.finalResultMissingTask->cancel();
		m_resultCalls.erase(it);
		break;
	default:
		logger().error("invalid result " + to_string(result->status())
			+ " for call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString(),
			__FILE__, __LINE__);

		return;
	}

	guard.unlock();

	executeResultCall(gatewayID, callID, resultCall, result);
}

void AsyncGatewayRPC::forwardResponse(const GatewayID &gatewayID,
		GWResponse::Ptr response)
{
	GatewayRPCResult::Ptr result = new GatewayRPCResult;

	switch (response->status()) {
	case GWResponse::Status::ACCEPTED:
		result->setStatus(GatewayRPCResult::Status::ACCEPTED);
		break;
	case GWResponse::Status::SUCCESS:
		result->setStatus(GatewayRPCResult::Status::SUCCESS);
		break;
	case GWResponse::Status::FAILED:
		result->setStatus(GatewayRPCResult::Status::FAILED);
		break;
	default:
		throw InvalidArgumentException("invalid type "
			+ to_string(response->status())
			+ " of response "
			+ gatewayID.toString()
			+ " : "
			+ response->id().toString(),
			__FILE__, __LINE__);
	}

	processResult(gatewayID, response->id(), result);
}

void AsyncGatewayRPC::forwardTimeout(const GatewayID &gatewayID,
		const GlobalID &responseID)
{
	GatewayRPCResult::Ptr result = new GatewayRPCResult;
	result->setStatus(GatewayRPCResult::Status::TIMEOUT);

	processResult(gatewayID, responseID, result);
}

void AsyncGatewayRPC::start()
{
	logger().information("gateway rpc is up", __FILE__, __LINE__);
}

void AsyncGatewayRPC::stop()
{
	logger().information("stopping gateway rpc", __FILE__, __LINE__);

	m_timer.cancel(true);

	FastMutex::ScopedLock guard(m_mutex);
	for (auto &it : m_resultCalls) {
		try {
			GatewayRPCResult::Ptr result = new GatewayRPCResult;
			result->setStatus(GatewayRPCResult::Status::FAILED);

			it.second.resultCall(result);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
		}
		catch (const exception &e) {
			logger().critical(e.what(), __FILE__, __LINE__);
		}
		catch (...) {
			logger().critical("unknown error", __FILE__, __LINE__);
		}
	}

	m_resultCalls.clear();

	logger().information("gateway rpc has stopped", __FILE__, __LINE__);
}

void AsyncGatewayRPC::setFinalResultTimeout(const Timespan &timeout)
{
	if (timeout < 0)
		throw InvalidArgumentException("result timeout must be non-negative");

	m_finalResultTimeout = timeout;
}

void AsyncGatewayRPC::setGatewayCommunicator(GatewayCommunicator::Ptr communicator)
{
	m_gatewayCommunicator = communicator;
}

void AsyncGatewayRPC::setGWResponseExpectedQueue(GWResponseExpectedQueue::Ptr queue)
{
	m_responseExpectedQueue = queue;
}
