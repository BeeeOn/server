#ifndef BEEEON_LOCATION_SERVICE_H
#define BEEEON_LOCATION_SERVICE_H

#include <vector>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "service/Single.h"
#include "service/Deserializer.h"
#include "dao/LocationDao.h"
#include "di/InjectorTarget.h"
#include "model/Place.h"
#include "model/Gateway.h"
#include "Debug.h"

namespace BeeeOn {

class GatewayDao;

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

	void setGatewayDao(GatewayDao *dao);

	void createIn(Location &location,
			const Deserializer<Location> &data,
			const Place &place);
	void createIn(Location &location,
			const Deserializer<Location> &data,
			const Gateway &gateway);
	bool fetch(Single<Location> &input);
	bool fetchFrom(Location &location, const Place &place);
	void fetchBy(std::vector<Location> &locations, const Gateway &gateway);
	bool updateIn(Location &location,
			const Deserializer<Location> &update,
			const Place &place);
	bool updateIn(Location &location,
			const Deserializer<Location> &update,
			const Gateway &gateway);
	bool remove(Single<Location> &input);
	bool removeFrom(Location &location, const Place &place);
	bool removeFrom(Location &location, const Gateway &gateway);

private:
	LocationDao *m_dao;
	GatewayDao *m_gatewayDao;
};

}

#endif
