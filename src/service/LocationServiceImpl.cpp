#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/LocationServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, LocationServiceImpl)
BEEEON_OBJECT_CASTABLE(LocationService)
BEEEON_OBJECT_PROPERTY("locationDao", &LocationServiceImpl::setLocationDao)
BEEEON_OBJECT_PROPERTY("gatewayDao", &LocationServiceImpl::setGatewayDao)
BEEEON_OBJECT_PROPERTY("accessPolicy", &LocationServiceImpl::setAccessPolicy)
BEEEON_OBJECT_PROPERTY("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, LocationServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

LocationServiceImpl::LocationServiceImpl()
{
}

void LocationServiceImpl::setLocationDao(LocationDao::Ptr dao)
{
	m_dao = dao;
}

void LocationServiceImpl::setGatewayDao(GatewayDao::Ptr dao)
{
	m_gatewayDao = dao;
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

	m_accessPolicy->assure(LocationAccessPolicy::ACTION_USER_CREATE, input, tmp);

	Location &location = input.target();
	input.data().full(location);
	location.setGateway(tmp);

	m_dao->create(location);
}

bool LocationServiceImpl::doFetchFrom(Relation<Location, Gateway> &input)
{
	m_accessPolicy->assure(LocationAccessPolicy::ACTION_USER_GET, input, input.target());

	return m_dao->fetchFrom(input.target(), input.base());
}

void LocationServiceImpl::doFetchBy(Relation<vector<Location>, Gateway> &input)
{
	m_dao->fetchBy(input.target(), input.base());
}

bool LocationServiceImpl::doUpdateIn(RelationWithData<Location, Gateway> &input)
{
	Location &location = input.target();

	m_accessPolicy->assure(LocationAccessPolicy::ACTION_USER_UPDATE, input, location);

	if (!m_dao->fetchFrom(location, input.base()))
		throw NotFoundException("location does not exist");

	input.data().partial(location);
	return m_dao->update(location);
}

bool LocationServiceImpl::doRemove(Single<Location> &input)
{
	Location &location = input.target();

	m_accessPolicy->assure(LocationAccessPolicy::ACTION_USER_REMOVE, input, location);

	if (!m_dao->fetch(location))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}

bool LocationServiceImpl::doRemoveFrom(Relation<Location, Gateway> &input)
{
	Location &location = input.target();

	m_accessPolicy->assure(LocationAccessPolicy::ACTION_USER_REMOVE, input, location);

	if (!m_dao->fetchFrom(location, input.base()))
		throw NotFoundException("location does not exist");

	return m_dao->remove(location);
}
