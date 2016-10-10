#ifndef BEEEON_PLACE_SERVICE_H
#define BEEEON_PLACE_SERVICE_H

#include "di/InjectorTarget.h"
#include "model/Place.h"
#include "model/Identity.h"
#include "model/User.h"
#include "dao/PlaceDao.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/VerifiedIdentityDao.h"

namespace BeeeOn {

class PlaceService : public AbstractInjectorTarget {
public:
	PlaceService():
		m_placeDao(&NullPlaceDao::instance()),
		m_roleInPlaceDao(&NullRoleInPlaceDao::instance())
	{
		injector<PlaceService, PlaceDao>("placeDao",
			&PlaceService::setPlaceDao);
		injector<PlaceService, RoleInPlaceDao>("roleInPlaceDao",
			&PlaceService::setRoleInPlaceDao);
		injector<PlaceService, VerifiedIdentityDao>(
			"verifiedIdentityDao",
			&PlaceService::setVerifiedIdentityDao
		);
	}

	void setPlaceDao(PlaceDao *dao)
	{
		if (dao == NULL)
			m_placeDao = &NullPlaceDao::instance();
		else
			m_placeDao = dao;
	}

	void setRoleInPlaceDao(RoleInPlaceDao *dao)
	{
		if (dao == NULL)
			m_roleInPlaceDao = &NullRoleInPlaceDao::instance();
		else
			m_roleInPlaceDao = dao;
	}

	void setVerifiedIdentityDao(VerifiedIdentityDao *dao)
	{
		if (dao == NULL)
			m_verifiedIdentityDao = &NullVerifiedIdentityDao::instance();
		else
			m_verifiedIdentityDao = dao;
	}

	void create(Place &place, const Identity &identity);
	void fetchAccessible(std::vector<Place> &places, const User &user);
	bool fetch(Place &place);
	bool update(Place &place);
	bool remove(Place &place, const User &owner);

private:
	PlaceDao *m_placeDao;
	RoleInPlaceDao *m_roleInPlaceDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
};

}

#endif
