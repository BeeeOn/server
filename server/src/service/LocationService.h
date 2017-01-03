#ifndef BEEEON_LOCATION_SERVICE_H
#define BEEEON_LOCATION_SERVICE_H

#include <vector>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "service/Single.h"
#include "service/Relation.h"
#include "di/InjectorTarget.h"
#include "model/Location.h"
#include "model/Place.h"
#include "model/Gateway.h"
#include "Debug.h"

namespace BeeeOn {

class LocationDao;
class GatewayDao;
class LocationAccessPolicy;

/**
 * Locations management.
 */
class LocationService : public AbstractInjectorTarget {
public:
	LocationService();

	void setLocationDao(LocationDao *dao);
	void setGatewayDao(GatewayDao *dao);
	void setAccessPolicy(LocationAccessPolicy *policy);

	void createIn(RelationWithData<Location, Place> &input);
	void createIn(RelationWithData<Location, Gateway> &input);
	bool fetch(Single<Location> &input);
	bool fetchFrom(Relation<Location, Place> &input);
	void fetchBy(Relation<std::vector<Location>, Gateway> &input);
	bool updateIn(RelationWithData<Location, Place> &input);
	bool updateIn(RelationWithData<Location, Gateway> &input);
	bool remove(Single<Location> &input);
	bool removeFrom(Relation<Location, Place> &input);
	bool removeFrom(Relation<Location, Gateway> &input);

private:
	LocationDao *m_dao;
	GatewayDao *m_gatewayDao;
	LocationAccessPolicy *m_accessPolicy;
};

}

#endif