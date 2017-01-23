#ifndef BEEEON_GATEWAY_DAO_H
#define BEEEON_GATEWAY_DAO_H

#include <vector>

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "model/User.h"
#include "model/Gateway.h"
#include "model/LegacyGateway.h"

namespace BeeeOn {

class GatewayDao {
public:
	virtual bool insert(Gateway &gateway) = 0;
	virtual bool fetch(Gateway &gateway) = 0;
	virtual bool fetch(LegacyGateway &gateway, const User &user) = 0;
	virtual bool update(Gateway &gateway) = 0;
	virtual bool assignAndUpdate(Gateway &gateway, const Place &place) = 0;
	virtual void fetchAccessible(
			std::vector<Gateway> &gateways,
			const User &user) = 0;
	virtual void fetchAccessible(
			std::vector<LegacyGateway> &gateways,
			const User &user) = 0;
};

class NullGatewayDao : public AbstractInjectorTarget,
	public NullDao<Gateway, GatewayDao> {
public:
	/**
	 * Provide a singleton instance to avoid
	 * unnecessary allocations.
	 */
	static GatewayDao &instance();

	bool fetch(LegacyGateway &gateway, const User &user)
	{
		throw Poco::NotImplementedException(__func__);
	}

	bool assignAndUpdate(Gateway &gateway, const Place &place)
	{
		return update(gateway);
	}

	void fetchAccessible(std::vector<Gateway> &gateways,
			const User &user)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void fetchAccessible(std::vector<LegacyGateway> &gateways,
			const User &user)
	{
		throw Poco::NotImplementedException(__func__);
	}
};

}

#endif
