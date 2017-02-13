#include <vector>
#include <Poco/Exception.h>

#include "service/GatewayService.h"
#include "server/AccessLevel.h"
#include "dao/GatewayDao.h"
#include "dao/RoleInGatewayDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "policy/GatewayAccessPolicy.h"

BEEEON_OBJECT(BeeeOn_GatewayService, BeeeOn::GatewayService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GatewayService::GatewayService():
	m_gatewayDao(&NullGatewayDao::instance()),
	m_roleInGatewayDao(&NullRoleInGatewayDao::instance()),
	m_identityDao(&NullIdentityDao::instance()),
	m_rpc(&NullGatewayRPC::instance()),
	m_accessPolicy(&NullGatewayAccessPolicy::instance())
{
	injector("gatewayDao", &GatewayService::setGatewayDao);
	injector("roleInGatewayDao", &GatewayService::setRoleInGatewayDao);
	injector("identityDao", &GatewayService::setIdentityDao);
	injector("verifiedIdentityDao", &GatewayService::setVerifiedIdentityDao);
	injector("gatewayRPC", &GatewayService::setGatewayRPC);
	injector("accessPolicy", &GatewayService::setAccessPolicy);
}

void GatewayService::setGatewayDao(GatewayDao *dao)
{
	m_gatewayDao = dao? dao : &NullGatewayDao::instance();
}

void GatewayService::setRoleInGatewayDao(RoleInGatewayDao *dao)
{
	m_roleInGatewayDao = dao? dao :&NullRoleInGatewayDao::instance();
}

void GatewayService::setIdentityDao(IdentityDao *dao)
{
	m_identityDao = dao? dao :&NullIdentityDao::instance();
}

void GatewayService::setVerifiedIdentityDao(VerifiedIdentityDao *dao)
{
	m_verifiedIdentityDao = dao? dao :
			&NullVerifiedIdentityDao::instance();
}

void GatewayService::setGatewayRPC(GatewayRPC *rpc)
{
	m_rpc = rpc? rpc : &NullGatewayRPC::instance();
}

void GatewayService::setAccessPolicy(GatewayAccessPolicy *policy)
{
	m_accessPolicy = policy? policy :
		&NullGatewayAccessPolicy::instance();
}

bool GatewayService::doRegisterGateway(SingleWithData<Gateway> &input,
		const VerifiedIdentity &verifiedIdentity)
{
	m_accessPolicy->assureRegister(input, input.target());

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

bool GatewayService::doFetch(Single<Gateway> &input)
{
	m_accessPolicy->assureGet(input, input.target());

	return m_gatewayDao->fetch(input.target());
}

bool GatewayService::doFetch(Single<LegacyGateway> &input)
{
	m_accessPolicy->assureGet(input, input.target());

	return m_gatewayDao->fetch(input.target(), input.user());
}

void GatewayService::doFetchAccessible(Relation<vector<Gateway>, User> &input)
{
	m_gatewayDao->fetchAccessible(input.target(), input.base());
}

void GatewayService::doFetchAccessible(
		Relation<vector<LegacyGateway>, User> &input)
{
	m_gatewayDao->fetchAccessible(input.target(), input.base());
}

bool GatewayService::doUpdate(SingleWithData<Gateway> &input)
{
	Gateway &gateway = input.target();

	m_accessPolicy->assureUpdate(input, gateway);

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	input.data().partial(gateway);
	return m_gatewayDao->update(gateway);
}

bool GatewayService::doUnregister(Single<Gateway> &input)
{
	Gateway &gateway = input.target();

	m_accessPolicy->assureUnregister(input, input.target());

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

void GatewayService::doScanDevices(Single<Gateway> &input)
{
	m_accessPolicy->assureScanDevices(input, input.target());

	m_rpc->sendListen(input.target());
}

void GatewayService::doUnpairDevice(Single<Gateway> &input, Device &device)
{
	m_rpc->unpairDevice(input.target(), device);
}

void GatewayService::doPingGateway(Single<Gateway> &input)
{
	m_rpc->pingGateway(input.target());
}
