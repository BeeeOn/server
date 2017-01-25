#ifndef BEEEON_GATEWAY_SERVICE_H
#define BEEEON_GATEWAY_SERVICE_H

#include <vector>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "service/Single.h"
#include "service/Relation.h"
#include "dao/Transactional.h"
#include "rpc/GatewayRPC.h"
#include "di/InjectorTarget.h"
#include "Debug.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"
#include "model/Gateway.h"
#include "model/LegacyGateway.h"

namespace BeeeOn {

class Place;
class GatewayDao;
class RoleInGatewayDao;
class IdentityDao;
class VerifiedIdentityDao;
class GatewayRPC;
class GatewayAccessPolicy;

class GatewayService : public Transactional {
public:
	GatewayService();

	void setGatewayDao(GatewayDao *dao);
	void setRoleInGatewayDao(RoleInGatewayDao *dao);
	void setIdentityDao(IdentityDao *dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao *dao);
	void setGatewayRPC(GatewayRPC *rpc);
	void setAccessPolicy(GatewayAccessPolicy *policy);

	/**
	 * Register the given gateway to be owned by the given identity.
	 *
	 * @throw NotFoundException when the gateway does not exist
	 * @throw InvalidArgumentException for invalid identity
	 *
	 * @return false when assignment fails (update operation fails)
	 */
	bool registerGateway(SingleWithData<Gateway> &input,
			const VerifiedIdentity &verifiedIdentity)
	{
		return BEEEON_TRANSACTION_RETURN(bool,
				doRegisterGateway(input, verifiedIdentity));
	}

	bool fetch(Single<Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	bool fetch(Single<LegacyGateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchAccessible(Relation<std::vector<Gateway>, User> &input)
	{
		BEEEON_TRANSACTION(doFetchAccessible(input));
	}

	void fetchAccessible(Relation<std::vector<LegacyGateway>, User> &input)
	{
		BEEEON_TRANSACTION(doFetchAccessible(input));
	}

	bool update(SingleWithData<Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdate(input));
	}

	bool unregister(Single<Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUnregister(input));
	}

	void scanDevices(Single<Gateway> &input)
	{
		doScanDevices(input);
	}

	void unpairDevice(Single<Gateway> &input, Device &device)
	{
		doUnpairDevice(input, device);
	}

	void pingGateway(Single<Gateway> &input)
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
	void doScanDevices(Single<Gateway> &input);
	void doUnpairDevice(Single<Gateway> &input, Device &device);
	void doPingGateway(Single<Gateway> &input);

private:
	GatewayDao *m_gatewayDao;
	RoleInGatewayDao *m_roleInGatewayDao;
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	GatewayRPC *m_rpc;
	GatewayAccessPolicy *m_accessPolicy;
};

}

#endif
