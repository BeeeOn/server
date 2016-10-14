#ifndef BEEEON_LOCATION_DAO_H
#define BEEEON_LOCATION_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "dao/GatewayDao.h"
#include "model/Place.h"
#include "model/Gateway.h"
#include "model/Location.h"

namespace BeeeOn {

class LocationDao {
public:
	virtual void create(Location &location) = 0;
	virtual bool fetch(Location &location) = 0;
	virtual bool fetchFrom(Location &location,
			const Place &place) = 0;
	virtual bool fetchFrom(Location &location,
			const Gateway &gateway) = 0;
	virtual bool update(Location &location) = 0;
	virtual bool remove(const Location &location) = 0;
};

class NullLocationDao : public AbstractInjectorTarget,
	public NullDao<Location, LocationDao> {
public:
	bool fetchFrom(Location &location,
			const Place &place)
	{
		return fetch(location);
	}

	bool fetchFrom(Location &location,
			const Gateway &gateway)
	{
		return fetch(location);
	}

	static LocationDao &instance();
};
class MockLocationDao : public AbstractInjectorTarget,
	public MockDao<Location, LocationDao> {
public:
	MockLocationDao();

	bool fetchFrom(Location &location,
			const Place &place)
	{
		Location tmp(location.id());

		if (!fetch(tmp))
			return false;

		if (tmp.place().id() != place.id())
			return false;

		location = tmp;
		return true;
	}

	bool fetchFrom(Location &location,
			const Gateway &gateway)
	{
		Location tmp(location.id());
		Gateway owner(gateway.id());

		if (!m_gatewayDao->fetch(owner))
			return false;

		if (!fetch(tmp))
			return false;

		if (owner.place().id() != tmp.place().id())
			return false;

		location = tmp;
		return true;
	}

	void setGatewayDao(GatewayDao *dao)
	{
		m_gatewayDao = dao;
	}

protected:
	LocationID nextID()
	{
		return LocationID::random();
	}

	GatewayDao *m_gatewayDao;
};

}

#endif
