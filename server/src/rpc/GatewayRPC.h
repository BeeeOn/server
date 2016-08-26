#ifndef BEEEON_GATEWAY_RPC_H
#define BEEEON_GATEWAY_RPC_H

#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "model/Gateway.h"
#include "model/Device.h"

namespace BeeeOn {

/**
 * Class GatewayRPC is used to access gateways remotely.
 * It allows to send primitive commands to gateways. Most of
 * the communication works asynchronously and the results are
 * not returned immediately but must be queries in other ways.
 */
class GatewayRPC {
public:
	/**
	 * Send listen command.
	 */
	virtual void sendListen(Gateway &gateway) = 0;

	/**
	 * Send unpair device command to the selected gateway.
	 */
	virtual void unpairDevice(Gateway &gateway, Device &device) = 0;

	/**
	 * Send ping command to the gateway.
	 */
	virtual void pingGateway(Gateway &gateway) = 0;
};

class NullGatewayRPC : public GatewayRPC, public AbstractInjectorTarget {
public:
	void sendListen(Gateway &gateway)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void unpairDevice(Gateway &gateway, Device &device)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void pingGateway(Gateway &gateway)
	{
		throw Poco::NotImplementedException(__func__);
	}

	static GatewayRPC &instance();
};

}

#endif
