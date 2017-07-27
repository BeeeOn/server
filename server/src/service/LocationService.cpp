#include <Poco/Exception.h>

#include "dao/LocationDao.h"
#include "dao/GatewayDao.h"
#include "di/Injectable.h"
#include "policy/LocationAccessPolicy.h"
#include "service/LocationService.h"

BEEEON_OBJECT_BEGIN(BeeeOn, LocationService)
BEEEON_OBJECT_REF("locationDao", &LocationService::setLocationDao)
BEEEON_OBJECT_REF("gatewayDao", &LocationService::setGatewayDao)
BEEEON_OBJECT_REF("accessPolicy", &LocationService::setAccessPolicy)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, LocationService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

LocationService::LocationService():
	m_dao(&NullLocationDao::instance()),
	m_gatewayDao(&NullGatewayDao::instance()),
	m_accessPolicy(&NullLocationAccessPolicy::instance())
{
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

	m_accessPolicy->assureCreateLocation(input, tmp);

	Location &location = input.target();
	input.data().full(location);
	location.setGateway(tmp);

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
