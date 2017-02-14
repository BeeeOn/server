#ifndef BEEEON_GATEWAY_RPC_H
#define BEEEON_GATEWAY_RPC_H

#include <Poco/Exception.h>

#include "di/AbstractInjectorTarget.h"
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
	virtual void sendListen(const Gateway &gateway) = 0;

	/**
	 * Send unpair device command to the selected gateway.
	 */
	virtual void unpairDevice(const Gateway &gateway,
			const Device &device) = 0;

	/**
	 * Send ping command to the gateway.
	 */
	virtual void pingGateway(const Gateway &gateway) = 0;
};

class NullGatewayRPC : public GatewayRPC, public AbstractInjectorTarget {
public:
	void sendListen(const Gateway &gateway)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void unpairDevice(const Gateway &gateway, const Device &device)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void pingGateway(const Gateway &gateway)
	{
		throw Poco::NotImplementedException(__func__);
	}

	static GatewayRPC &instance();
};

}

#endif
