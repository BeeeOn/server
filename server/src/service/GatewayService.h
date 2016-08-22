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
	GatewayService():
		m_dao(&NullGatewayDao::instance())
	{
		injector<GatewayService, GatewayDao>("gatewayDao",
				&GatewayService::setGatewayDao);
	}

	void setGatewayDao(GatewayDao *dao)
	{
		if (dao == NULL)
			m_dao = &NullGatewayDao::instance();
		else
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
