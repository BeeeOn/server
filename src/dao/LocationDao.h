#pragma once

#include <vector>

#include <Poco/SharedPtr.h>

#include "dao/GatewayDao.h"
#include "model/Gateway.h"
#include "model/Location.h"

namespace BeeeOn {

class LocationDao {
public:
	typedef Poco::SharedPtr<LocationDao> Ptr;

	virtual ~LocationDao();

	virtual void create(Location &location) = 0;
	virtual bool fetch(Location &location) = 0;
	virtual bool fetchFrom(Location &location,
			const Gateway &gateway) = 0;
	virtual void fetchBy(std::vector<Location> &locations,
			const Gateway &gateway) = 0;
	virtual bool update(Location &location) = 0;
	virtual bool remove(const Location &location) = 0;
};

}
