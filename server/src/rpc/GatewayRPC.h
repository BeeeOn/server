#ifndef BEEEON_GATEWAY_RPC_H
#define BEEEON_GATEWAY_RPC_H

#include <functional>

#include <Poco/Exception.h>
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
	typedef std::function<void(GatewayRPCResult::Ptr)> ResultCall;

	/**
	 * Send listen command.
	 */
	virtual void sendListen(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Poco::Timespan &duration) = 0;

	/**
	 * Send unpair device command to the selected gateway.
	 */
	virtual void unpairDevice(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device) = 0;

	/**
	 * Send ping command to the gateway.
	 */
	virtual void pingGateway(
			const ResultCall &resultCall,
			const Gateway &gateway) = 0;

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

class NullGatewayRPC : public GatewayRPC {
public:
	void sendListen(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Poco::Timespan &duration)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void unpairDevice(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void pingGateway(const ResultCall &resultCall, const Gateway &gateway)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void updateActor(
			const ResultCall &resultCall,
			const Gateway &gateway,
			const Device &device,
			const ModuleInfo &module,
			double value,
			const Poco::Timespan &timeout)
	{
		throw Poco::NotImplementedException(__func__);
	}

	static GatewayRPC &instance();
};

}

#endif
