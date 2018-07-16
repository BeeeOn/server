#include <vector>

#include <Poco/Event.h>

#include "di/Injectable.h"
#include "server/AccessLevel.h"
#include "service/GatewayServiceImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GatewayServiceImpl)
BEEEON_OBJECT_CASTABLE(GatewayService)
BEEEON_OBJECT_CASTABLE(GWSGatewayService)
BEEEON_OBJECT_PROPERTY("gatewayDao", &GatewayServiceImpl::setGatewayDao)
BEEEON_OBJECT_PROPERTY("gatewayStatusDao", &GatewayServiceImpl::setGatewayStatusDao)
BEEEON_OBJECT_PROPERTY("roleInGatewayDao", &GatewayServiceImpl::setRoleInGatewayDao)
BEEEON_OBJECT_PROPERTY("identityDao", &GatewayServiceImpl::setIdentityDao)
BEEEON_OBJECT_PROPERTY("verifiedIdentityDao", &GatewayServiceImpl::setVerifiedIdentityDao)
BEEEON_OBJECT_PROPERTY("scanController", &GatewayServiceImpl::setScanController)
BEEEON_OBJECT_PROPERTY("accessPolicy", &GatewayServiceImpl::setAccessPolicy)
BEEEON_OBJECT_PROPERTY("transactionManager", &GatewayServiceImpl::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, GatewayServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GatewayServiceImpl::GatewayServiceImpl()
{
}

void GatewayServiceImpl::setGatewayDao(GatewayDao::Ptr dao)
{
	m_gatewayDao = dao;
}

void GatewayServiceImpl::setGatewayStatusDao(GatewayStatusDao::Ptr dao)
{
	m_gatewayStatusDao = dao;
}

void GatewayServiceImpl::setRoleInGatewayDao(RoleInGatewayDao::Ptr dao)
{
	m_roleInGatewayDao = dao;
}

void GatewayServiceImpl::setIdentityDao(IdentityDao::Ptr dao)
{
	m_identityDao = dao;
}

void GatewayServiceImpl::setVerifiedIdentityDao(VerifiedIdentityDao::Ptr dao)
{
	m_verifiedIdentityDao = dao;
}

void GatewayServiceImpl::setScanController(GatewayScanController::Ptr controller)
{
	m_scanController = controller;
}

void GatewayServiceImpl::setAccessPolicy(GatewayAccessPolicy::Ptr policy)
{
	m_accessPolicy = policy;
}

bool GatewayServiceImpl::doRegisterGateway(SingleWithData<Gateway> &input,
		const VerifiedIdentity &verifiedIdentity)
{
	m_accessPolicy->assure(GatewayAccessPolicy::ACTION_USER_REGISTER, input, input.target());

	VerifiedIdentity tmp(verifiedIdentity);

	if (!m_verifiedIdentityDao->fetch(tmp))
		throw InvalidArgumentException("invalid verified identity");

	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway was not found");

	input.data().full(gateway);

	RoleInGateway role;
	role.setGateway(gateway);
	role.setIdentity(tmp.identity());
	role.setLevel(AccessLevel::admin());
	m_roleInGatewayDao->create(role);

	return m_gatewayDao->update(gateway);
}

bool GatewayServiceImpl::doFetch(Single<Gateway> &input)
{
	m_accessPolicy->assure(GatewayAccessPolicy::ACTION_USER_GET, input, input.target());

	return m_gatewayDao->fetch(input.target());
}

bool GatewayServiceImpl::doFetch(Single<LegacyGateway> &input)
{
	m_accessPolicy->assure(GatewayAccessPolicy::ACTION_USER_GET, input, input.target());

	return m_gatewayDao->fetch(input.target(), input.user());
}

void GatewayServiceImpl::doFetchAccessible(Relation<vector<Gateway>, User> &input)
{
	m_gatewayDao->fetchAccessible(input.target(), input.base());
}

void GatewayServiceImpl::doFetchAccessible(
		Relation<vector<LegacyGateway>, User> &input)
{
	m_gatewayDao->fetchAccessible(input.target(), input.base());
}

bool GatewayServiceImpl::doUpdate(SingleWithData<Gateway> &input)
{
	Gateway &gateway = input.target();

	m_accessPolicy->assure(GatewayAccessPolicy::ACTION_USER_UPDATE, input, gateway);

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	input.data().partial(gateway);
	return m_gatewayDao->update(gateway);
}

bool GatewayServiceImpl::doUnregister(Single<Gateway> &input)
{
	Gateway &gateway = input.target();

	m_accessPolicy->assure(GatewayAccessPolicy::ACTION_USER_UNREGISTER, input, input.target());

	// if there would be only roles without admin access level
	// remove all the roles (unregister fro mthe gateway entirely)
	if (m_roleInGatewayDao->hasOnlyNonAdminExcept(gateway, input.user()))
		m_roleInGatewayDao->removeAll(gateway);
	// if there are admins or current user is the last user
	// remove only the current user's roles
	else
		return m_roleInGatewayDao->remove(gateway, input.user());

	return true;
}

GatewayScan GatewayServiceImpl::doScanDevices(Single<Gateway> &input, const Timespan &timeout)
{
	BEEEON_TRANSACTION(
		m_accessPolicy->assure(GatewayAccessPolicy::ACTION_USER_SCAN, input, input.target()));

	return m_scanController->scan(input.target(), timeout);
}

GatewayScan GatewayServiceImpl::doScanStatus(Single<Gateway> &input)
{
	BEEEON_TRANSACTION(
		m_accessPolicy->assure(GatewayAccessPolicy::ACTION_USER_SCAN, input, input.target()));

	return m_scanController->find(input.target().id());
}

bool GatewayServiceImpl::doRegisterGateway(GatewayStatus &status,
		Gateway &gateway)
{
	status.setLastChanged(DateTime());

	if (m_gatewayDao->fetch(gateway)) {
		m_gatewayStatusDao->insert(status, gateway);
		return true;
	}

	if (!m_gatewayDao->insert(gateway))
		return false;

	return m_gatewayStatusDao->insert(status, gateway);
}
