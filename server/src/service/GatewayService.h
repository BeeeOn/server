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
class RoleInPlaceDao;
class PlaceDao;
class IdentityDao;
class VerifiedIdentityDao;
class GatewayRPC;
class GatewayAccessPolicy;

class GatewayService : public Transactional {
public:
	GatewayService();

	void setGatewayDao(GatewayDao *dao);
	void setRoleInPlaceDao(RoleInPlaceDao *dao);
	void setPlaceDao(PlaceDao *dao);
	void setIdentityDao(IdentityDao *dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao *dao);
	void setGatewayRPC(GatewayRPC *rpc);
	void setAccessPolicy(GatewayAccessPolicy *policy);

	/**
	 * Register the given gateway to be owned by the given identity.
	 * If there is no place for this identity (or its assocated user)
	 * create a new implicit one.
	 *
	 * @throw NotFoundException when the gateway does not exist
	 * @throw IllegalStateException when there are more then 1 place
	 *	associated with the identity
	 * @throw InvalidArgumentException for invalid identity
	 * @throw ExistsException when the place is already registered
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

	bool unregister(Relation<Gateway, User> &input)
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
	bool doUnregister(Relation<Gateway, User> &input);
	void doScanDevices(Single<Gateway> &input);
	void doUnpairDevice(Single<Gateway> &input, Device &device);
	void doPingGateway(Single<Gateway> &input);

	void createImplicitPlace(Place &place,
			const Gateway &gateway,
			const Identity &identity);

private:
	GatewayDao *m_gatewayDao;
	RoleInPlaceDao *m_roleInPlaceDao;
	PlaceDao *m_placeDao;
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	GatewayRPC *m_rpc;
	GatewayAccessPolicy *m_accessPolicy;
};

}

#endif
