#ifndef BEEEON_GATEWAY_SERVICE_H
#define BEEEON_GATEWAY_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "dao/GatewayDao.h"
#include "rpc/GatewayRPC.h"
#include "di/InjectorTarget.h"
#include "Debug.h"
#include "model/Place.h"
#include "model/Gateway.h"

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
		return m_dao->fetch(gateway);
	}

	bool fetchFromPlace(Gateway &gateway, Place &place)
	{
		return m_dao->fetchFromPlace(gateway, place);
	}

	bool update(Gateway &gateway)
	{
		return m_dao->update(gateway);
	}

	bool assignAndUpdate(Gateway &gateway, const Place &place)
	{
		if (gateway.hasPlace())
			return false;

		return m_dao->assignAndUpdate(gateway, place);
	}

	bool unassign(Gateway &gateway, Place &place)
	{
		if (!m_dao->fetchFromPlace(gateway, place))
			return false;

		return m_dao->unassign(gateway);
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
