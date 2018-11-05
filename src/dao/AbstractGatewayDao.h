#pragma once

#include "dao/GatewayDao.h"

namespace BeeeOn {

/**
 * @brief AbstractGatewayDao provides a way how to manipulate,
 * validate or just watch instance of class Gateway without
 * knowing the exact storage implementation.
 */
class AbstractGatewayDao : public GatewayDao {
public:
	bool insert(Gateway &gateway) override;
	bool fetch(Gateway &gateway) override;
	bool fetch(LegacyGateway &gateway, const User &user) override;
	bool update(Gateway &gateway) override;
	void fetchAccessible(
		std::vector<Gateway> &gateways,
		const User &user) override;
	void fetchAccessible(
		std::vector<LegacyGateway> &gateways,
		const User &user) override;

protected:
	virtual bool doInsert(Gateway &gateway) = 0;
	virtual bool doFetch(Gateway &gateway) = 0;
	virtual bool doFetch(LegacyGateway &gateway, const User &user) = 0;
	virtual bool doUpdate(Gateway &gateway) = 0;
	virtual void doFetchAccessible(
		std::vector<Gateway> &gateways,
		const User &user) = 0;
	virtual void doFetchAccessible(
		std::vector<LegacyGateway> &gateways,
		const User &user) = 0;
};

}
