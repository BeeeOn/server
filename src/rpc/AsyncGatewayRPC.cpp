#include "di/Injectable.h"
#include "gwmessage/GWDeviceAcceptRequest.h"
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

void AsyncGatewayRPC::sendListen(GatewayRPCHandler::Ptr handler,
		const Gateway &gateway,
		const Timespan &duration)
{
	CallID callID = CallID::random();

	GWListenRequest::Ptr request = new GWListenRequest;
	request->setID(callID);
	request->setDuration(duration);

	sendAndExpectResult(gateway.id(), callID, handler, request);
}

void AsyncGatewayRPC::pairDevice(GatewayRPCHandler::Ptr handler,
		const Gateway &gateway,
		const Device &device)
{
	CallID callID = CallID::random();

	GWDeviceAcceptRequest::Ptr request = new GWDeviceAcceptRequest;
	request->setID(callID);
	request->setDeviceID(device.id());

	sendAndExpectResult(gateway.id(), callID, handler, request);
}

void AsyncGatewayRPC::unpairDevice(GatewayRPCHandler::Ptr handler,
		const Gateway &gateway,
		const Device &device)
{
	CallID callID = CallID::random();

	GWUnpairRequest::Ptr request = new GWUnpairRequest;
	request->setID(callID);
	request->setDeviceID(device.id());

	sendAndExpectResult(gateway.id(), callID, handler, request);
}

void AsyncGatewayRPC::updateActor(GatewayRPCHandler::Ptr handler,
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

	sendAndExpectResult(gateway.id(), callID, handler, request);
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

		auto it = m_contexts.find(make_pair(gatewayID, callID));
		if (it == m_contexts.end())
			return;

		GatewayRPCHandler::Ptr handler = it->second.handler;

		m_contexts.erase(it);
		guard.unlock();

		GatewayRPCResult::Ptr result = new GatewayRPCResult();
		result->setStatus(GatewayRPCResult::Status::TIMEOUT);

		executeHandler(gatewayID, callID, handler, result);
	});
}

void AsyncGatewayRPC::storeHandler(const GatewayID &gatewayID,
		const CallID &callID,
		GatewayRPCHandler::Ptr handler)
{
	Context context {
		handler,
		createFinalResultMissingTask(gatewayID, callID)
	};

	FastMutex::ScopedLock guard(m_mutex);

	auto it = m_contexts.emplace(make_pair(gatewayID, callID), context);
	if (!it.second) {
		throw IllegalStateException("duplicate result call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString());
	}

	m_timer.schedule(
		context.finalResultMissingTask, Timestamp() + m_finalResultTimeout);
}

void AsyncGatewayRPC::removeHandler(const GatewayID &gatewayID,
		const CallID &callID)
{
	FastMutex::ScopedLock guard(m_mutex);

	auto it = m_contexts.find(make_pair(gatewayID, callID));
	if (it == m_contexts.end()) {
		logger().warning("result call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString()
			+ " was not found",
			__FILE__, __LINE__);

		return;
	}

	it->second.finalResultMissingTask->cancel();
	m_contexts.erase(it);
}

void AsyncGatewayRPC::executeHandler(const GatewayID &gatewayID,
	const AsyncGatewayRPC::CallID &callID,
	GatewayRPCHandler::Ptr handler,
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
		doHandle(handler, result);
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
		GatewayRPCHandler::Ptr handler,
		const GWRequest::Ptr request)
{
	GatewayRPCResult::Ptr result = new GatewayRPCResult;

	storeHandler(gatewayID, callID, handler);
	m_responseExpectedQueue->registerResponse(gatewayID, callID);

	try {
		m_gatewayCommunicator->sendMessage(gatewayID, request);
	}
	catch (const NotFoundException &e) {
		logger().log(e, __FILE__, __LINE__);

		removeHandler(gatewayID, callID);
		m_responseExpectedQueue->notifyDelivered(gatewayID, callID);

		result->setStatus(GatewayRPCResult::Status::NOT_CONNECTED);

		m_timer.schedule(new LambdaTimerTask([=]() {
			executeHandler(gatewayID, callID, handler, result);
		}), Clock{});
	}
	catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);

		removeHandler(gatewayID, callID);
		m_responseExpectedQueue->notifyDelivered(gatewayID, callID);

		result->setStatus(GatewayRPCResult::Status::FAILED);

		m_timer.schedule(new LambdaTimerTask([=]() {
			executeHandler(gatewayID, callID, handler, result);
		}), Clock{});
	}
}

void AsyncGatewayRPC::processResult(const GatewayID &gatewayID,
		const CallID &callID,
		GatewayRPCResult::Ptr result)
{
	ScopedLockWithUnlock<FastMutex> guard(m_mutex);

	auto it = m_contexts.find(make_pair(gatewayID, callID));
	if (it == m_contexts.end()) {
		logger().warning("result call "
			+ gatewayID.toString()
			+ " : "
			+ callID.toString()
			+ " was not found",
			__FILE__, __LINE__);

		return;
	}

	GatewayRPCHandler::Ptr handler = it->second.handler;

	switch (result->status()) {
	case GatewayRPCResult::Status::ACCEPTED:
		break;
	case GatewayRPCResult::Status::TIMEOUT:
	case GatewayRPCResult::Status::SUCCESS:
	case GatewayRPCResult::Status::FAILED:
		it->second.finalResultMissingTask->cancel();
		m_contexts.erase(it);
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

	executeHandler(gatewayID, callID, handler, result);
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
	for (auto &it : m_contexts) {
		try {
			GatewayRPCResult::Ptr result = new GatewayRPCResult;
			result->setStatus(GatewayRPCResult::Status::FAILED);

			doHandle(it.second.handler, result);
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

	m_contexts.clear();

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
