#ifndef BEEEON_ASYNC_GATEWAY_RPC_H
#define BEEEON_ASYNC_GATEWAY_RPC_H

#include <Poco/SharedPtr.h>
#include <Poco/Util/Timer.h>

#include "gwmessage/GWRequest.h"
#include "gws/GatewayCommunicator.h"
#include "gws/GWResponseExpectedQueue.h"
#include "gws/RPCForwarder.h"
#include "loop/StoppableLoop.h"
#include "model/GlobalID.h"
#include "rpc/GatewayRPC.h"
#include "rpc/GatewayRPCResult.h"
#include "util/Loggable.h"
#include "util/LambdaTimerTask.h"

namespace BeeeOn {

/**
 * @brief AsyncGatewayRPC provides asynchronous GatewayRPC intended to be used
 * in the BeeeOn Server running as monolit. It allows to send commands to the
 * gateways and receive partial results asynchronously.
 *
 * After sending request to the gateway, the ResultCall is stored and it is
 * executed with every received partial result. If final result (TIMEOUT,
 * SUCCESS, FAILED) is not received till timeout, the stored ResultCall is
 * executed with result TIMEOUT.
 *
 * The final result means that request has been completed (with success or error)
 * or timeout has expired.
 */
class AsyncGatewayRPC :
	public GatewayRPC,
	public RPCForwarder,
	public StoppableLoop,
	protected Loggable {
public:
	typedef Poco::SharedPtr<AsyncGatewayRPC> Ptr;

	struct ResultCallContext {
		ResultCall resultCall;
		LambdaTimerTask::Ptr finalResultMissingTask;
	};

	typedef GlobalID CallID;
	typedef std::map<std::pair<GatewayID, CallID>, ResultCallContext> ResultCallMap;

	void sendListen(const ResultCall &resultCall,
			const Gateway &gateway,
			const Poco::Timespan &duration) override;

	void unpairDevice(const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device) override;

	void pingGateway(const ResultCall &resultCall,
			const Gateway &gateway) override;

	void updateActor(const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device,
			const ModuleInfo &module,
			double value,
			const Poco::Timespan &timeout) override;

	void forwardResponse(const GatewayID &gatewayID,
			GWResponse::Ptr response) override;
	void forwardTimeout(const GatewayID &gatewayID,
			const GlobalID &responseID) override;

	void start() override;
	void stop() override;

	void setFinalResultTimeout(const Poco::Timespan &timeout);
	void setGatewayCommunicator(GatewayCommunicator::Ptr communicator);
	void setGWResponseExpectedQueue(GWResponseExpectedQueue::Ptr queue);

private:
	/**
	 * @brief Creates a final result missing task and returns it.
	 *
	 * The task removes the ResultCall from the map and executes it with TIMEOUT.
	 */
	LambdaTimerTask::Ptr createFinalResultMissingTask(
			const GatewayID &gatewayID, const CallID &callID);

	/**
	 * @brief Stores the ResultCall and schedule final result missing task.
	 */
	void storeResultCall(const GatewayID &gatewayID,
			const CallID &callID,
			const ResultCall &resultCall);

	/**
	 * @brief Removes the ResultCall and cancels the final result missing task.
	 */
	void removeResultCall(const GatewayID &gatewayID, const CallID &callID);

	/**
	 * @brief Stores the ResultCall and sends the request to the Gateway.
	 *
	 * If gateway is not connected or sending fails, executes the ResultCall
	 * with NOT_CONNECTED resp. FAILED result.
	 */
	void sendAndExpectResult(const GatewayID &gatewayID,
			const CallID &callID,
			const ResultCall &resultCall,
			const GWRequest::Ptr request);

	/**
	 * @brief Finds the ResultCall for the given result and executes it.
	 */
	void processResult(const GatewayID &gatewayID,
			const GlobalID &callID,
			GatewayRPCResult::Ptr result);

	/**
	 * @brief Calls the ResultCall with given result.
	 */
	void executeResultCall(const GatewayID &gatewayID,
			const CallID &callID,
			const ResultCall &resultCall,
			GatewayRPCResult::Ptr result);

private:
	Poco::Timespan m_finalResultTimeout;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	GWResponseExpectedQueue::Ptr m_responseExpectedQueue;

	ResultCallMap m_resultCalls;
	Poco::FastMutex m_mutex;
	Poco::Util::Timer m_timer;
};

}

#endif
