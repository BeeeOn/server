#ifndef BEEEON_GATEWAY_SERVICE_H
#define BEEEON_GATEWAY_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "dao/GatewayDao.h"
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

class GatewayService : public AbstractInjectorTarget {
public:
	GatewayService()
	{
		injector<GatewayService, GatewayDao>("gatewayDao",
				&GatewayService::setGatewayDao);
	}

	void setGatewayDao(GatewayDao *dao)
	{
		m_dao = dao;
	}

	bool fetch(Gateway &gateway)
	{
		TRACE_METHOD();
		return m_dao->fetch(gateway);
	}

private:
	GatewayDao *m_dao;
};

}

#endif
