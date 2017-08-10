#ifndef BEEEON_GATEWAY_DAO_H
#define BEEEON_GATEWAY_DAO_H

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/User.h"
#include "model/Gateway.h"
#include "model/LegacyGateway.h"

namespace BeeeOn {

class GatewayDao {
public:
	typedef Poco::SharedPtr<GatewayDao> Ptr;

	virtual ~GatewayDao();

	virtual bool insert(Gateway &gateway) = 0;
	virtual bool fetch(Gateway &gateway) = 0;
	virtual bool fetch(LegacyGateway &gateway, const User &user) = 0;
	virtual bool update(Gateway &gateway) = 0;
	virtual void fetchAccessible(
			std::vector<Gateway> &gateways,
			const User &user) = 0;
	virtual void fetchAccessible(
			std::vector<LegacyGateway> &gateways,
			const User &user) = 0;
};

}

#endif
