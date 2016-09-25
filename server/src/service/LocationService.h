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
	LocationService():
		m_dao(&NullLocationDao::instance())
	{
		injector<LocationService, LocationDao>("locationDao",
				&LocationService::setLocationDao);
	}

	void setLocationDao(LocationDao *dao)
	{
		if (dao == NULL)
			m_dao = &NullLocationDao::instance();
		else
			m_dao = dao;
	}

	void createIn(Location &location, const Place &place)
	{
		TRACE_METHOD();

		location.setPlace(place);
		m_dao->create(location);
	}

	bool fetch(Location &location)
	{
		TRACE_METHOD();
		return m_dao->fetch(location);
	}

	bool fetchFrom(Location &location, const Place &place)
	{
		TRACE_METHOD();
		return m_dao->fetchFrom(location, place);
	}

	bool update(Location &location)
	{
		TRACE_METHOD();
		return m_dao->update(location);
	}

	bool remove(const Location &location)
	{
		TRACE_METHOD();
		return m_dao->remove(location);
	}

private:
	LocationDao *m_dao;
};

}

#endif
