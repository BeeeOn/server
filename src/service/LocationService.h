#ifndef BEEEON_LOCATION_SERVICE_H
#define BEEEON_LOCATION_SERVICE_H

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/Location.h"
#include "model/Gateway.h"
#include "service/Single.h"
#include "service/Relation.h"

namespace BeeeOn {

/**
 * Locations management.
 */
class LocationService {
public:
	typedef Poco::SharedPtr<LocationService> Ptr;

	virtual ~LocationService();

	virtual void createIn(RelationWithData<Location, Gateway> &input) = 0;
	virtual bool fetchFrom(Relation<Location, Gateway> &input) = 0;
	virtual void fetchBy(Relation<std::vector<Location>, Gateway> &input) = 0;
	virtual bool updateIn(RelationWithData<Location, Gateway> &input) = 0;
	virtual bool remove(Single<Location> &input) = 0;
	virtual bool removeFrom(Relation<Location, Gateway> &input) = 0;
};

}

#endif
