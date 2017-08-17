#ifndef BEEEON_LOCATION_SERVICE_IMPL_H
#define BEEEON_LOCATION_SERVICE_IMPL_H

#include "dao/GatewayDao.h"
#include "dao/LocationDao.h"
#include "policy/LocationAccessPolicy.h"
#include "service/LocationService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

/**
 * Locations management.
 */
class LocationServiceImpl : public LocationService, public Transactional {
public:
	LocationServiceImpl();

	void setLocationDao(LocationDao::Ptr dao);
	void setGatewayDao(GatewayDao::Ptr dao);
	void setAccessPolicy(LocationAccessPolicy::Ptr policy);

	void createIn(RelationWithData<Location, Gateway> &input)
	{
		BEEEON_TRANSACTION(doCreateIn(input));
	}

	bool fetchFrom(Relation<Location, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetchFrom(input));
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
	bool doFetchFrom(Relation<Location, Gateway> &input);
	void doFetchBy(Relation<std::vector<Location>, Gateway> &input);
	bool doUpdateIn(RelationWithData<Location, Gateway> &input);
	bool doRemove(Single<Location> &input);
	bool doRemoveFrom(Relation<Location, Gateway> &input);

private:
	LocationDao::Ptr m_dao;
	GatewayDao::Ptr m_gatewayDao;
	LocationAccessPolicy::Ptr m_accessPolicy;
};

}

#endif
