#ifndef BEEEON_GATEWAY_SERVICE_H
#define BEEEON_GATEWAY_SERVICE_H

#include <vector>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "service/Single.h"
#include "service/Relation.h"
#include "dao/GatewayDao.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/PlaceDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "rpc/GatewayRPC.h"
#include "di/InjectorTarget.h"
#include "Debug.h"
#include "model/User.h"
#include "model/VerifiedIdentity.h"
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

	void setRoleInPlaceDao(RoleInPlaceDao *dao)
	{
		if (dao == NULL)
			m_roleInPlaceDao = &NullRoleInPlaceDao::instance();
		else
			m_roleInPlaceDao = dao;
	}

	void setPlaceDao(PlaceDao *dao)
	{
		if (dao == NULL)
			m_placeDao = &NullPlaceDao::instance();
		else
			m_placeDao = dao;
	}

	void setIdentityDao(IdentityDao *dao)
	{
		if (dao == NULL)
			m_identityDao = &NullIdentityDao::instance();
		else
			m_identityDao = dao;
	}

	void setVerifiedIdentityDao(VerifiedIdentityDao *dao)
	{
		if (dao == NULL)
			m_verifiedIdentityDao =
				&NullVerifiedIdentityDao::instance();
		else
			m_verifiedIdentityDao = dao;
	}

	void setGatewayRPC(GatewayRPC *rpc)
	{
		if (rpc == NULL)
			m_rpc = &NullGatewayRPC::instance();
		else
			m_rpc = rpc;
	}

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
			const VerifiedIdentity &verifiedIdentity);

	bool fetch(Single<Gateway> &input);
	bool fetchFromPlace(Relation<Gateway, Place> &input);
	void fetchAccessible(Relation<std::vector<Gateway>, User> &input);

	bool update(SingleWithData<Gateway> &input);
	bool updateInPlace(RelationWithData<Gateway, Place> &input);
	bool assignAndUpdate(RelationWithData<Gateway, Place> &input);
	bool unassign(Relation<Gateway, Place> &input);
	bool unassign(Relation<Gateway, User> &input);

	void scanDevices(Single<Gateway> &input);
	void unpairDevice(Single<Gateway> &input, Device &device);
	void pingGateway(Single<Gateway> &input);

protected:
	void createImplicitPlace(Place &place, Identity &identity);

private:
	GatewayDao *m_gatewayDao;
	RoleInPlaceDao *m_roleInPlaceDao;
	PlaceDao *m_placeDao;
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	GatewayRPC *m_rpc;
};

}

#endif
