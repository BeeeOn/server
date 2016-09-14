#ifndef BEEEON_LOCATION_DAO_H
#define BEEEON_LOCATION_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/Place.h"
#include "model/Location.h"

namespace BeeeOn {

class LocationDao {
public:
	virtual void create(Location &location) = 0;
	virtual bool fetch(Location &location) = 0;
	virtual bool fetchFrom(Location &location,
			const Place &place) = 0;
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

	static LocationDao &instance();
};
class MockLocationDao : public AbstractInjectorTarget,
	public MockDao<Location, LocationDao> {
public:
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

protected:
	LocationID nextID()
	{
		return LocationID::random();
	}
};

}

#endif
