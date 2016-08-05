#ifndef BEEEON_LOCATION_SERVICE_H
#define BEEEON_LOCATION_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "dao/LocationDao.h"
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Locations management.
 */
class LocationService : public AbstractInjectorTarget {
public:
	LocationService()
	{
		injector<LocationService, LocationDao>("locationDao",
				&LocationService::setLocationDao);
	}

	void setLocationDao(LocationDao *dao)
	{
		m_dao = dao;
	}

	bool fetch(Location &location)
	{
		TRACE_METHOD();
		return m_dao->fetch(location);
	}

private:
	LocationDao *m_dao;
};

}

#endif
