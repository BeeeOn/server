#ifndef BEEEON_GATEWAY_SERVICE_IMPL_H
#define BEEEON_GATEWAY_SERVICE_IMPL_H

#include <vector>

#include "dao/GatewayDao.h"
#include "dao/IdentityDao.h"
#include "dao/RoleInGatewayDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "policy/GatewayAccessPolicy.h"
#include "rpc/GatewayRPC.h"
#include "service/GatewayService.h"
#include "transaction/Transactional.h"
#include "work/WorkFacade.h"

namespace Poco {

class Timespan;

}

namespace BeeeOn {

class IdentityDao;

class GatewayServiceImpl : public GatewayService, public Transactional {
public:
	GatewayServiceImpl();

	void setGatewayDao(GatewayDao::Ptr dao);
	void setRoleInGatewayDao(RoleInGatewayDao::Ptr dao);
	void setIdentityDao(IdentityDao::Ptr dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao::Ptr dao);
	void setGatewayRPC(GatewayRPC::Ptr rpc);
	void setWorkFacade(WorkFacade::Ptr facade);
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

	Work scanDevices(Single<Gateway> &input, const Poco::Timespan &duration) override
	{
		return BEEEON_TRANSACTION_RETURN(Work, doScanDevices(input, duration));
	}

	void pingGateway(Single<Gateway> &input) override
	{
		doPingGateway(input);
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
	Work doScanDevices(Single<Gateway> &input, const Poco::Timespan &duration);
	void doPingGateway(Single<Gateway> &input);

private:
	GatewayDao::Ptr m_gatewayDao;
	RoleInGatewayDao::Ptr m_roleInGatewayDao;
	IdentityDao::Ptr m_identityDao;
	VerifiedIdentityDao::Ptr m_verifiedIdentityDao;
	GatewayRPC::Ptr m_rpc;
	WorkFacade::Ptr m_workFacade;
	GatewayAccessPolicy::Ptr m_accessPolicy;
};

}

#endif

