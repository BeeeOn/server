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

	Location::Ptr get(LocationID id, PlaceID placeId)
	{
		TRACE_METHOD();
		return m_dao->get(id);
	}

private:
	LocationDao *m_dao;
};

}

#endif
