#ifndef BEEEON_LOCATION_SERVICE_H
#define BEEEON_LOCATION_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "service/Deserializer.h"
#include "dao/LocationDao.h"
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Locations management.
 */
class LocationService : public AbstractInjectorTarget {
public:
	LocationService();

	void setLocationDao(LocationDao *dao)
	{
		if (dao == NULL)
			m_dao = &NullLocationDao::instance();
		else
			m_dao = dao;
	}

	void createIn(Location &location,
			const Deserializer<Location> &data,
			const Place &place);
	bool fetch(Location &location);
	bool fetchFrom(Location &location, const Place &place);
	bool updateIn(Location &location,
			const Deserializer<Location> &update,
			const Place &place);
	bool remove(const Location &location);
	bool removeFrom(Location &location, const Place &place);

private:
	LocationDao *m_dao;
};

}

#endif
