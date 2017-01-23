#include <Poco/Exception.h>

#include "service/LocationService.h"
#include "dao/LocationDao.h"
#include "dao/GatewayDao.h"
#include "policy/LocationAccessPolicy.h"

BEEEON_OBJECT(LocationService, BeeeOn::LocationService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

LocationService::LocationService():
	m_dao(&NullLocationDao::instance()),
	m_gatewayDao(&NullGatewayDao::instance()),
	m_accessPolicy(&NullLocationAccessPolicy::instance())
{
	injector<LocationService, LocationDao>("locationDao",
			&LocationService::setLocationDao);
	injector<LocationService, GatewayDao>("gatewayDao",
			&LocationService::setGatewayDao);
	injector<LocationService, LocationAccessPolicy>("accessPolicy",
			&LocationService::setAccessPolicy);
}

void LocationService::setLocationDao(LocationDao *dao)
{
	m_dao = dao? dao : &NullLocationDao::instance();
}

void LocationService::setGatewayDao(GatewayDao *dao)
{
	m_gatewayDao = dao? dao : &NullGatewayDao::instance();
}

void LocationService::setAccessPolicy(LocationAccessPolicy *policy)
{
	m_accessPolicy = policy? policy :
		&NullLocationAccessPolicy::instance();
}

void LocationService::doCreateIn(RelationWithData<Location, Gateway> &input)
{
	Gateway tmp(input.base());

	if (!m_gatewayDao->fetch(tmp))
		throw NotFoundException("gateway does not exist");

	if (!tmp.hasPlace()) // do not leak it exists
		throw NotFoundException("gateway is not assigned");

	m_accessPolicy->assureCreateLocation(input, tmp.place());

	Location &location = input.target();
	input.data().full(location);
	location.setPlace(tmp.place());

	m_dao->create(location);
}

bool LocationService::doFetch(Single<Location> &input)
{
	m_accessPolicy->assureGet(input, input.target());

	return m_dao->fetch(input.target());
}

void LocationService::doFetchBy(Relation<vector<Location>, Gateway> &input)
{
	m_dao->fetchBy(input.target(), input.base());
}

bool LocationService::doUpdateIn(RelationWithData<Location, Gateway> &input)
{
	Location &location = input.target();

	m_accessPolicy->assureUpdate(input, location);

	if (!m_dao->fetchFrom(location, input.base()))
		throw NotFoundException("location does not exist");

	input.data().partial(location);
	return m_dao->update(location);
}

bool LocationService::doRemove(Single<Location> &input)
{
	Location &location = input.target();

	m_accessPolicy->assureRemove(input, location);

	if (!m_dao->fetch(location))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}

bool LocationService::doRemoveFrom(Relation<Location, Gateway> &input)
{
	Location &location = input.target();

	m_accessPolicy->assureRemove(input, location);

	if (!m_dao->fetchFrom(location, input.base()))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}
