#ifndef BEEEON_GATEWAY_SERVICE_H
#define BEEEON_GATEWAY_SERVICE_H

#include <vector>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "service/Deserializer.h"
#include "dao/GatewayDao.h"
#include "rpc/GatewayRPC.h"
#include "di/InjectorTarget.h"
#include "Debug.h"
#include "model/User.h"
#include "model/Place.h"
#include "model/Gateway.h"

namespace BeeeOn {

class GatewayService : public AbstractInjectorTarget {
public:
	GatewayService();

	void setGatewayDao(GatewayDao *dao)
	{
		if (dao == NULL)
			m_gatewayDao = &NullGatewayDao::instance();
		else
			m_gatewayDao = dao;
	}

	void setGatewayRPC(GatewayRPC *rpc)
	{
		if (rpc == NULL)
			m_rpc = &NullGatewayRPC::instance();
		else
			m_rpc = rpc;
	}

	bool fetch(Gateway &gateway);
	bool fetchFromPlace(Gateway &gateway, const Place &place);
	void fetchAccessible(std::vector<Gateway> &gateways,
			const User &user);

	bool update(Gateway &gateway);
	bool updateInPlace(Gateway &gateway,
			const Deserializer<Gateway> &update,
			const Place &place);
	bool assignAndUpdate(Gateway &gateway,
			const Deserializer<Gateway> &update,
			const Place &place);
	bool unassign(Gateway &gateway, const Place &place);

	void scanDevices(Gateway &gateway);
	void unpairDevice(Gateway &gateway, Device &device);
	void pingGateway(Gateway &gateway);

private:
	GatewayDao *m_gatewayDao;
	GatewayRPC *m_rpc;
};

}

#endif
