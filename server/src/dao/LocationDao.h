#ifndef BEEEON_LOCATION_DAO_H
#define BEEEON_LOCATION_DAO_H

#include <vector>

#include "di/AbstractInjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/GatewayDao.h"
#include "model/Gateway.h"
#include "model/Location.h"

namespace BeeeOn {

class LocationDao {
public:
	virtual void create(Location &location) = 0;
	virtual bool fetch(Location &location) = 0;
	virtual bool fetchFrom(Location &location,
			const Gateway &gateway) = 0;
	virtual void fetchBy(std::vector<Location> &locations,
			const Gateway &gateway) = 0;
	virtual bool update(Location &location) = 0;
	virtual bool remove(const Location &location) = 0;
};

class NullLocationDao : public AbstractInjectorTarget,
	public NullDao<Location, LocationDao> {
public:
	bool fetchFrom(Location &location,
			const Gateway &gateway)
	{
		return fetch(location);
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
