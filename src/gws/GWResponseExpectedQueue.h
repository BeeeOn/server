#pragma once

#include <Poco/Timespan.h>
#include <Poco/Mutex.h>
#include <Poco/SharedPtr.h>
#include <Poco/Util/Timer.h>
#include <Poco/Util/TimerTask.h>

#include "gws/GatewayCommunicator.h"
#include "gws/RPCForwarder.h"
#include "loop/StoppableLoop.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief This class is responsible for reaction on missing expected response
 * from the gateway and detection of the broken connection.
 *
 * Each request sent to the gateway expect appropriate response. If response
 * is not received till timeout, this fact must be forwarded to the UIServer.
 * All expected responses are registered to this queue and unregistered after
 * arriving. The unique response identifier is stored with response missing task
 * object, which is scheduled in Poco::Util::Timer. The task represents reaction
 * on the missing response, which means forwarding timeout response via the
 * RPCForwarder and removing connection from the GatewayCommunicator, if it is
 * inactive.
 */
class GWResponseExpectedQueue :
	public StoppableLoop,
	protected Loggable {
public:
	typedef Poco::SharedPtr<GWResponseExpectedQueue> Ptr;
	typedef std::map<std::pair<GatewayID, GlobalID>,
		Poco::Util::TimerTask::Ptr> ResponseMap;

	void start() override;
	void stop() override;

	void setResponseTimeout(const Poco::Timespan &timeout);
	void setGatewayCommunicator(GatewayCommunicator::Ptr communicator);
	void setRPCForwarder(RPCForwarder::Ptr forwarder);

	/**
	 * @brief Add expected response to the queue
	 * and schedule response missing task.
	 */
	void registerResponse(const GatewayID &gatewayID, const GlobalID &responseID);

	/**
	 * @brief Removes expected response from the queue
	 * and cancel response missing task.
	 */
	void notifyDelivered(const GatewayID &gatewayID, const GlobalID &responseID);

	/**
	 * Break dependency cycles prior to destruction.
	 */
	void cleanup();

private:
	/**
	 * @brief Creates a response missing task and returns it.
	 *
	 * The task forwards timeout response using RPCForwarder and remove
	 * the gateway connection from the GatewayCommunicator if it is inactive.
	 */
	Poco::Util::TimerTask::Ptr createResponseMissingTask(const GatewayID &gatewayID,
		const GlobalID &responseID);

private:
	Poco::Timespan m_responseTimeout;
	GatewayCommunicator::Ptr m_gatewayCommunicator;
	RPCForwarder::Ptr m_rpcForwarder;

	ResponseMap m_responses;
	Poco::FastMutex m_mutex;
	Poco::Util::Timer m_timer;
};

}
