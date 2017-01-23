#include <vector>
#include <Poco/Exception.h>

#include "service/GatewayService.h"
#include "server/AccessLevel.h"
#include "dao/GatewayDao.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/RoleInGatewayDao.h"
#include "dao/PlaceDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "policy/GatewayAccessPolicy.h"

BEEEON_OBJECT(GatewayService, BeeeOn::GatewayService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GatewayService::GatewayService():
	m_gatewayDao(&NullGatewayDao::instance()),
	m_roleInPlaceDao(&NullRoleInPlaceDao::instance()),
	m_roleInGatewayDao(&NullRoleInGatewayDao::instance()),
	m_placeDao(&NullPlaceDao::instance()),
	m_identityDao(&NullIdentityDao::instance()),
	m_rpc(&NullGatewayRPC::instance()),
	m_accessPolicy(&NullGatewayAccessPolicy::instance())
{
	injector<GatewayService, GatewayDao>("gatewayDao",
			&GatewayService::setGatewayDao);
	injector<GatewayService, RoleInPlaceDao>("roleInPlaceDao",
			&GatewayService::setRoleInPlaceDao);
	injector<GatewayService, RoleInGatewayDao>("roleInGatewayDao",
			&GatewayService::setRoleInGatewayDao);
	injector<GatewayService, PlaceDao>("placeDao",
			&GatewayService::setPlaceDao);
	injector<GatewayService, IdentityDao>("identityDao",
			&GatewayService::setIdentityDao);
	injector<GatewayService, VerifiedIdentityDao>("verifiedIdentityDao",
			&GatewayService::setVerifiedIdentityDao);
	injector<GatewayService, GatewayRPC>("gatewayRPC",
			&GatewayService::setGatewayRPC);
	injector<GatewayService, GatewayAccessPolicy>("accessPolicy",
			&GatewayService::setAccessPolicy);
}

void GatewayService::setGatewayDao(GatewayDao *dao)
{
	m_gatewayDao = dao? dao : &NullGatewayDao::instance();
}

void GatewayService::setRoleInPlaceDao(RoleInPlaceDao *dao)
{
	m_roleInPlaceDao = dao? dao :&NullRoleInPlaceDao::instance();
}

void GatewayService::setRoleInGatewayDao(RoleInGatewayDao *dao)
{
	m_roleInGatewayDao = dao? dao :&NullRoleInGatewayDao::instance();
}

void GatewayService::setPlaceDao(PlaceDao *dao)
{
	m_placeDao = dao? dao : &NullPlaceDao::instance();
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
	VerifiedIdentity tmp(verifiedIdentity);

	if (!m_verifiedIdentityDao->fetch(tmp))
		throw InvalidArgumentException("invalid verified identity");

	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway was not found");

	if (gateway.hasPlace()) {
		const AccessLevel &level = m_roleInPlaceDao->
			fetchAccessLevel(gateway.place(), tmp.user());

		if (level <= AccessLevel::admin()) // is owner
			throw ExistsException("gateway is already assigned");

		throw NotFoundException("gateway is owned by somebody else");
	}

	input.data().full(gateway);

	RoleInGateway role;
	role.setGateway(gateway);
	role.setIdentity(tmp.identity());
	role.setLevel(AccessLevel::admin());
	m_roleInGatewayDao->create(role);

	Place place;
	createImplicitPlace(place, gateway, tmp.identity());
	return m_gatewayDao->assignAndUpdate(gateway, place);
}

void GatewayService::createImplicitPlace(
		Place &place,
		const Gateway &gateway,
		const Identity &identity)
{
	place.setName(string("Place for ") + gateway.name());

	m_placeDao->create(place);

	RoleInPlace role;
	role.setPlace(place);
	role.setIdentity(identity);
	role.setLevel(AccessLevel::admin());

	m_roleInPlaceDao->create(role);
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

bool GatewayService::doUnregister(Relation<Gateway, User> &input)
{
	Gateway &gateway = input.target();

	m_accessPolicy->assureUnregister(input, input.target());

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	if (!gateway.hasPlace()) // do not leak that this gateway exists
		throw NotFoundException("gateway is not assigned");

	return m_gatewayDao->unassign(gateway);
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
