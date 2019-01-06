#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Util/Timer.h>

#include "dao/DevicePropertyDao.h"
#include "gwmessage/GWRequest.h"
#include "gws/GatewayCommunicator.h"
#include "gws/GWResponseExpectedQueue.h"
#include "gws/RPCForwarder.h"
#include "loop/StoppableLoop.h"
#include "model/GlobalID.h"
#include "rpc/GatewayRPC.h"
#include "rpc/GatewayRPCResult.h"
#include "transaction/Transactional.h"
#include "util/CryptoConfig.h"
#include "util/Loggable.h"
#include "util/LambdaTimerTask.h"

namespace BeeeOn {

/**
 * @brief AsyncGatewayRPC provides asynchronous GatewayRPC intended to be used
 * in the BeeeOn Server running as monolit. It allows to send commands to the
 * gateways and receive partial results asynchronously.
 *
 * After sending request to the gateway, the GatewayRPCHandler instance is stored
 * and it is executed with every received partial result. If final result (TIMEOUT,
 * SUCCESS, FAILED) is not received till timeout, the stored handler is executed
 * with result TIMEOUT.
 *
 * The final result means that request has been completed (with success or error)
 * or timeout has expired.
 */
class AsyncGatewayRPC :
	public GatewayRPC,
	public RPCForwarder,
	public StoppableLoop,
	public Transactional {
public:
	typedef Poco::SharedPtr<AsyncGatewayRPC> Ptr;

	struct Context {
		GatewayRPCHandler::Ptr handler;
		LambdaTimerTask::Ptr finalResultMissingTask;
	};

	typedef GlobalID CallID;
	typedef std::map<std::pair<GatewayID, CallID>, Context> ContextMap;

	void sendListen(GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Poco::Timespan &duration) override;

	void pairDevice(GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Device &device) override;

	void unpairDevice(GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Device &device) override;

	void updateActor(GatewayRPCHandler::Ptr handler,
			const Gateway &gateway,
			const Device &device,
			const ModuleInfo &module,
			double value,
			const Poco::Timespan &timeout,
			const OpMode &mode) override;

	void forwardResponse(const GatewayID &gatewayID,
			GWResponse::Ptr response) override;
	void forwardTimeout(const GatewayID &gatewayID,
			const GlobalID &responseID) override;

	void start() override;
	void stop() override;

	void setDefaultTimeout(const Poco::Timespan &timeout);
	void setGatewayCommunicator(GatewayCommunicator::Ptr communicator);
	void setGWResponseExpectedQueue(GWResponseExpectedQueue::Ptr queue);
	void setDevicePropertyDao(DevicePropertyDao::Ptr dao);
	void setCryptoConfig(Poco::SharedPtr<CryptoConfig> config);

private:
	/**
	 * @brief Creates a final result missing task and returns it.
	 *
	 * The task removes the registered GatewayRPCHandler instance from the context
	 * map and executes it with TIMEOUT.
	 */
	LambdaTimerTask::Ptr createFinalResultMissingTask(
			const GatewayID &gatewayID, const CallID &callID);

	/**
	 * @brief Stores the handler and schedule final result missing task.
	 */
	void storeHandler(const GatewayID &gatewayID,
			const CallID &callID,
			GatewayRPCHandler::Ptr handler,
			const Poco::Timespan &timeout);

	/**
	 * @brief Removes the appropriate GatewayRPCHandler instance and cancels
	 * the final result missing task.
	 */
	void removeHandler(const GatewayID &gatewayID, const CallID &callID);

	/**
	 * @brief Stores the handler and sends the request to the Gateway.
	 *
	 * If gateway is not connected or sending fails, executes the
	 * GatewayRPCHandler::onNotConnected() or GatewayRPCHandler::onFailed().
	 */
	void sendAndExpectResult(const GatewayID &gatewayID,
			const CallID &callID,
			GatewayRPCHandler::Ptr handler,
			const GWRequest::Ptr request,
			const Poco::Timespan &timeout);

	/**
	 * @brief Derive GatewayRPCResult instance based on the received response.
	 */
	GatewayRPCResult::Ptr deriveResult(const GWResponse::Ptr response) const;

	/**
	 * @brief Finds the appropriate GatewayRPCHandler instance for the given
	 * result and uses it for handling.
	 */
	void processResult(const GatewayID &gatewayID,
			const GlobalID &callID,
			GatewayRPCResult::Ptr result);

	/**
	 * @brief Calls the given handler with given result.
	 */
	void executeHandler(const GatewayID &gatewayID,
			const CallID &callID,
			GatewayRPCHandler::Ptr handler,
			GatewayRPCResult::Ptr result);

private:
	Poco::Timespan m_defaultTimeout;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWResponseExpectedQueue::Ptr m_responseExpectedQueue;
	DevicePropertyDao::Ptr m_propertyDao;
	Poco::SharedPtr<CryptoConfig> m_cryptoConfig;

	ContextMap m_contexts;
	Poco::FastMutex m_mutex;
	Poco::Util::Timer m_timer;
};

}
