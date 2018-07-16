#ifndef BEEEON_GATEWAY_SERVICE_IMPL_H
#define BEEEON_GATEWAY_SERVICE_IMPL_H

#include <vector>

#include "dao/GatewayDao.h"
#include "dao/GatewayStatusDao.h"
#include "dao/IdentityDao.h"
#include "dao/RoleInGatewayDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "gws/GatewayScanController.h"
#include "policy/GatewayAccessPolicy.h"
#include "rpc/GatewayRPC.h"
#include "service/GatewayService.h"
#include "service/GWSGatewayService.h"
#include "transaction/Transactional.h"

namespace Poco {

class Timespan;

}

namespace BeeeOn {

class IdentityDao;

class GatewayServiceImpl : public GatewayService, public GWSGatewayService, public Transactional {
public:
	GatewayServiceImpl();

	void setGatewayDao(GatewayDao::Ptr dao);
	void setGatewayStatusDao(GatewayStatusDao::Ptr dao);
	void setRoleInGatewayDao(RoleInGatewayDao::Ptr dao);
	void setIdentityDao(IdentityDao::Ptr dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao::Ptr dao);
	void setScanController(GatewayScanController::Ptr controller);
	void setAccessPolicy(GatewayAccessPolicy::Ptr policy);

	/**
	 * Register the given gateway to be owned by the given identity.
	 *
	 * @throw NotFoundException when the gateway does not exist
	 * @throw InvalidArgumentException for invalid identity
	 *
	 * @return false when assignment fails (update operation fails)
	 */
	bool registerGateway(SingleWithData<Gateway> &input,
			const VerifiedIdentity &verifiedIdentity) override
	{
		return BEEEON_TRANSACTION_RETURN(bool,
				doRegisterGateway(input, verifiedIdentity));
	}

	bool fetch(Single<Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	bool fetch(Single<LegacyGateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchAccessible(Relation<std::vector<Gateway>, User> &input) override
	{
		BEEEON_TRANSACTION(doFetchAccessible(input));
	}

	void fetchAccessible(Relation<std::vector<LegacyGateway>, User> &input) override
	{
		BEEEON_TRANSACTION(doFetchAccessible(input));
	}

	bool update(SingleWithData<Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdate(input));
	}

	bool unregister(Single<Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUnregister(input));
	}

	GatewayScan scanDevices(Single<Gateway> &input, const Poco::Timespan &duration) override
	{
		return doScanDevices(input, duration);
	}

	GatewayScan scanStatus(Single<Gateway> &input) override
	{
		return doScanStatus(input);
	}

	bool registerGateway(GatewayStatus &status, Gateway &gateway) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRegisterGateway(status, gateway));
	}

protected:
	bool doRegisterGateway(SingleWithData<Gateway> &input,
			const VerifiedIdentity &verifiedIdentity);
	bool doFetch(Single<Gateway> &input);
	bool doFetch(Single<LegacyGateway> &input);
	void doFetchAccessible(Relation<std::vector<Gateway>, User> &input);
	void doFetchAccessible(Relation<std::vector<LegacyGateway>, User> &input);
	bool doUpdate(SingleWithData<Gateway> &input);
	bool doUnregister(Single<Gateway> &input);
	GatewayScan doScanDevices(Single<Gateway> &input, const Poco::Timespan &duration);
	GatewayScan doScanStatus(Single<Gateway> &input);

	bool doRegisterGateway(GatewayStatus &status, Gateway &gateway);

private:
	GatewayDao::Ptr m_gatewayDao;
	GatewayStatusDao::Ptr m_gatewayStatusDao;
	RoleInGatewayDao::Ptr m_roleInGatewayDao;
	IdentityDao::Ptr m_identityDao;
	VerifiedIdentityDao::Ptr m_verifiedIdentityDao;
	GatewayAccessPolicy::Ptr m_accessPolicy;
	GatewayScanController::Ptr m_scanController;
};

}

#endif

