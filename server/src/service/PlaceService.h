#ifndef BEEEON_PLACE_SERVICE_H
#define BEEEON_PLACE_SERVICE_H

#include "di/InjectorTarget.h"
#include "service/Single.h"
#include "service/Relation.h"
#include "model/Place.h"
#include "model/VerifiedIdentity.h"
#include "model/Identity.h"
#include "model/User.h"

namespace BeeeOn {

class PlaceDao;
class RoleInPlaceDao;
class VerifiedIdentityDao;
class PlaceAccessPolicy;

class PlaceService : public AbstractInjectorTarget {
public:
	PlaceService();

	void setPlaceDao(PlaceDao *dao);
	void setRoleInPlaceDao(RoleInPlaceDao *dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao *dao);
	void setAccessPolicy(PlaceAccessPolicy *policy);

	void create(SingleWithData<Place> &input,
			const Identity &identity);
	void create(SingleWithData<Place> &input,
			VerifiedIdentity &identity);
	void fetchAccessible(Relation<std::vector<Place>, User> &input);
	bool fetch(Single<Place> &input);
	bool update(SingleWithData<Place> &input);
	bool remove(Relation<Place, User> &input);

private:
	PlaceDao *m_placeDao;
	RoleInPlaceDao *m_roleInPlaceDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	PlaceAccessPolicy *m_accessPolicy;
};

}

#endif
