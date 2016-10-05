#ifndef BEEEON_PLACE_SERVICE_H
#define BEEEON_PLACE_SERVICE_H

#include "di/InjectorTarget.h"
#include "model/Place.h"
#include "model/Identity.h"
#include "model/User.h"
#include "dao/PlaceDao.h"
#include "dao/RoleInPlaceDao.h"

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

	void create(Place &place, const Identity &identity)
	{
		m_placeDao->create(place);

		RoleInPlace role;
		role.setPlace(place);
		role.setIdentity(identity);
		role.setLevel(AccessLevel::admin());

		m_roleInPlaceDao->create(role);
	}

	void fetchAccessible(std::vector<Place> &places,
			const User &user)
	{
		m_roleInPlaceDao->fetchAccessiblePlaces(places, user);
	}

	bool fetch(Place &place)
	{
		return m_placeDao->fetch(place);
	}

	bool update(Place &place)
	{
		return m_placeDao->update(place);
	}

	bool remove(Place &place, const User &owner);

private:
	PlaceDao *m_placeDao;
	RoleInPlaceDao *m_roleInPlaceDao;
};

}

#endif
