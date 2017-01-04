#ifndef BEEEON_LOCATION_DAO_H
#define BEEEON_LOCATION_DAO_H

#include <vector>

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
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
	virtual void fetchBy(std::vector<Location> &locations,
			const Place &place) = 0;
	virtual void fetchBy(std::vector<Location> &locations,
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

	void fetchBy(std::vector<Location> &locations,
			const Place &place)
	{
		throw Poco::NotImplementedException(__func__);
	}

	void fetchBy(std::vector<Location> &locations,
			const Gateway &gateway)
	{
		throw Poco::NotImplementedException(__func__);
	}

	static LocationDao &instance();
};

}

#endif
