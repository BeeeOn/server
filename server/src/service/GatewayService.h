#ifndef BEEEON_GATEWAY_SERVICE_H
#define BEEEON_GATEWAY_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "dao/GatewayDao.h"
#include "rpc/GatewayRPC.h"
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

class GatewayService : public AbstractInjectorTarget {
public:
	GatewayService():
		m_dao(&NullGatewayDao::instance()),
		m_rpc(&NullGatewayRPC::instance())
	{
		injector<GatewayService, GatewayDao>("gatewayDao",
				&GatewayService::setGatewayDao);
		injector<GatewayService, GatewayRPC>("gatewayRPC",
				&GatewayService::setGatewayRPC);
	}

	void setGatewayDao(GatewayDao *dao)
	{
		if (dao == NULL)
			m_dao = &NullGatewayDao::instance();
		else
			m_dao = dao;
	}

	void setGatewayRPC(GatewayRPC *rpc)
	{
		if (rpc == NULL)
			m_rpc = &NullGatewayRPC::instance();
		else
			m_rpc = rpc;
	}

	bool fetch(Gateway &gateway)
	{
		TRACE_METHOD();
		return m_dao->fetch(gateway);
	}

	void scanDevices(Gateway &gateway)
	{
		m_rpc->sendListen(gateway);
	}

	void unpairDevice(Gateway &gateway, Device &device)
	{
		m_rpc->unpairDevice(gateway, device);
	}

	void pingGateway(Gateway &gateway)
	{
		m_rpc->pingGateway(gateway);
	}

private:
	GatewayDao *m_dao;
	GatewayRPC *m_rpc;
};

}

#endif
