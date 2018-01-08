#ifndef BEEEON_GATEWAY_RPC_H
#define BEEEON_GATEWAY_RPC_H

#include <functional>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "model/Gateway.h"
#include "model/Device.h"
#include "model/ModuleInfo.h"
#include "rpc/GatewayRPCResult.h"

namespace BeeeOn {

/**
 * Class GatewayRPC is used to access gateways remotely.
 * It allows to send primitive commands to gateways. Most of
 * the communication works asynchronously and the results are
 * not returned immediately but must be queries in other ways.
 */
class GatewayRPC {
public:
	typedef Poco::SharedPtr<GatewayRPC> Ptr;

	typedef std::function<void(GatewayRPCResult::Ptr)> ResultCall;

	virtual ~GatewayRPC();

	/**
	 * Send listen command.
	 */
	virtual void sendListen(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Poco::Timespan &duration) = 0;

	/**
	 * Send pair device command to the selected gateway.
	 */
	virtual void pairDevice(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device) = 0;

	/**
	 * Send unpair device command to the selected gateway.
	 */
	virtual void unpairDevice(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device) = 0;

	/**
	 * Send update command for the given module.
	 */
	virtual void updateActor(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device,
			const ModuleInfo &module,
			double value,
			const Poco::Timespan &timeout) = 0;
};

}

#endif
