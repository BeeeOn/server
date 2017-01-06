#ifndef BEEEON_PLACE_SERVICE_H
#define BEEEON_PLACE_SERVICE_H

#include "di/InjectorTarget.h"
#include "service/Single.h"
#include "service/Relation.h"
#include "dao/Transactional.h"
#include "model/Place.h"
#include "model/VerifiedIdentity.h"
#include "model/Identity.h"
#include "model/User.h"

namespace BeeeOn {

class PlaceDao;
class RoleInPlaceDao;
class VerifiedIdentityDao;
class PlaceAccessPolicy;

class PlaceService : public Transactional {
public:
	PlaceService();

	void setPlaceDao(PlaceDao *dao);
	void setRoleInPlaceDao(RoleInPlaceDao *dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao *dao);
	void setAccessPolicy(PlaceAccessPolicy *policy);

	void create(SingleWithData<Place> &input, const Identity &identity)
	{
		BEEEON_TRANSACTION(doCreate(input, identity));
	}

	void create(SingleWithData<Place> &input, VerifiedIdentity &identity)
	{
		BEEEON_TRANSACTION(doCreate(input, identity));
	}

	bool fetch(Single<Place> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchAccessible(Relation<std::vector<Place>, User> &input)
	{
		BEEEON_TRANSACTION(doFetchAccessible(input));
	}

	bool update(SingleWithData<Place> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdate(input));
	}

	bool remove(Relation<Place, User> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRemove(input));
	}

protected:
	void doCreate(SingleWithData<Place> &input, const Identity &identity);
	void doCreate(SingleWithData<Place> &input, VerifiedIdentity &identity);
	bool doFetch(Single<Place> &input);
	void doFetchAccessible(Relation<std::vector<Place>, User> &input);
	bool doUpdate(SingleWithData<Place> &input);
	bool doRemove(Relation<Place, User> &input);

private:
	PlaceDao *m_placeDao;
	RoleInPlaceDao *m_roleInPlaceDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	PlaceAccessPolicy *m_accessPolicy;
};

}

#endif
