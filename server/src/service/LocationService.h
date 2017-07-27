#ifndef BEEEON_LOCATION_SERVICE_H
#define BEEEON_LOCATION_SERVICE_H

#include <vector>

#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "model/Location.h"
#include "model/Gateway.h"
#include "service/Single.h"
#include "service/Relation.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class LocationDao;
class GatewayDao;
class LocationAccessPolicy;

/**
 * Locations management.
 */
class LocationService : public Transactional {
public:
	LocationService();

	void setLocationDao(LocationDao *dao);
	void setGatewayDao(GatewayDao *dao);
	void setAccessPolicy(LocationAccessPolicy *policy);

	void createIn(RelationWithData<Location, Gateway> &input)
	{
		BEEEON_TRANSACTION(doCreateIn(input));
	}

	bool fetch(Single<Location> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchBy(Relation<std::vector<Location>, Gateway> &input)
	{
		BEEEON_TRANSACTION(doFetchBy(input));
	}

	bool updateIn(RelationWithData<Location, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdateIn(input));
	}

	bool remove(Single<Location> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRemove(input));
	}

	bool removeFrom(Relation<Location, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRemoveFrom(input));
	}

protected:
	void doCreateIn(RelationWithData<Location, Gateway> &input);
	bool doFetch(Single<Location> &input);
	void doFetchBy(Relation<std::vector<Location>, Gateway> &input);
	bool doUpdateIn(RelationWithData<Location, Gateway> &input);
	bool doRemove(Single<Location> &input);
	bool doRemoveFrom(Relation<Location, Gateway> &input);

private:
	LocationDao *m_dao;
	GatewayDao *m_gatewayDao;
	LocationAccessPolicy *m_accessPolicy;
};

}

#endif
