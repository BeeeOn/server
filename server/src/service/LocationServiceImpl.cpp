#include <Poco/Exception.h>

#include "dao/LocationDao.h"
#include "dao/GatewayDao.h"
#include "di/Injectable.h"
#include "service/LocationServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, LocationServiceImpl)
BEEEON_OBJECT_CASTABLE(LocationService)
BEEEON_OBJECT_REF("locationDao", &LocationServiceImpl::setLocationDao)
BEEEON_OBJECT_REF("gatewayDao", &LocationServiceImpl::setGatewayDao)
BEEEON_OBJECT_REF("accessPolicy", &LocationServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, LocationServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

LocationServiceImpl::LocationServiceImpl():
	m_dao(&NullLocationDao::instance()),
	m_gatewayDao(&NullGatewayDao::instance())
{
}

void LocationServiceImpl::setLocationDao(LocationDao *dao)
{
	m_dao = dao? dao : &NullLocationDao::instance();
}

void LocationServiceImpl::setGatewayDao(GatewayDao *dao)
{
	m_gatewayDao = dao? dao : &NullGatewayDao::instance();
}

void LocationServiceImpl::setAccessPolicy(LocationAccessPolicy::Ptr policy)
{
	m_accessPolicy = policy;
}

void LocationServiceImpl::doCreateIn(RelationWithData<Location, Gateway> &input)
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

bool LocationServiceImpl::doFetch(Single<Location> &input)
{
	m_accessPolicy->assureGet(input, input.target());

	return m_dao->fetch(input.target());
}

void LocationServiceImpl::doFetchBy(Relation<vector<Location>, Gateway> &input)
{
	m_dao->fetchBy(input.target(), input.base());
}

bool LocationServiceImpl::doUpdateIn(RelationWithData<Location, Gateway> &input)
{
	Location &location = input.target();

	m_accessPolicy->assureUpdate(input, location);

	if (!m_dao->fetchFrom(location, input.base()))
		throw NotFoundException("location does not exist");

	input.data().partial(location);
	return m_dao->update(location);
}

bool LocationServiceImpl::doRemove(Single<Location> &input)
{
	Location &location = input.target();

	m_accessPolicy->assureRemove(input, location);

	if (!m_dao->fetch(location))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}

bool LocationServiceImpl::doRemoveFrom(Relation<Location, Gateway> &input)
{
	Location &location = input.target();

	m_accessPolicy->assureRemove(input, location);

	if (!m_dao->fetchFrom(location, input.base()))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}
