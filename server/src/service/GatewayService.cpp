#include <vector>
#include <Poco/Exception.h>

#include "service/GatewayService.h"
#include "server/AccessLevel.h"
#include "dao/GatewayDao.h"
#include "dao/RoleInPlaceDao.h"
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
	m_placeDao(&NullPlaceDao::instance()),
	m_identityDao(&NullIdentityDao::instance()),
	m_rpc(&NullGatewayRPC::instance()),
	m_accessPolicy(&NullGatewayAccessPolicy::instance())
{
	injector<GatewayService, GatewayDao>("gatewayDao",
			&GatewayService::setGatewayDao);
	injector<GatewayService, RoleInPlaceDao>("roleInPlaceDao",
			&GatewayService::setRoleInPlaceDao);
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

bool GatewayService::registerGateway(SingleWithData<Gateway> &input,
		const VerifiedIdentity &verifiedIdentity)
{
	VerifiedIdentity tmp(verifiedIdentity);

	if (!m_verifiedIdentityDao->fetch(tmp))
		throw InvalidArgumentException("invalid verified identity");

	Identity identity(tmp.identity());

	if (!m_identityDao->fetch(identity))
		throw InvalidArgumentException("invalid identity given: "
				+ identity.repr());

	if (!identity.hasUser())
		throw InvalidArgumentException("identity with no user");

	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway was not found");

	if (gateway.hasPlace()) {
		const AccessLevel &level = m_roleInPlaceDao->
			fetchAccessLevel(gateway.place(), identity.user());

		if (level <= AccessLevel::admin()) // is owner
			throw ExistsException("gateway is already assigned");

		throw NotFoundException("gateway is owned by somebody else");
	}

	vector<Place> places;
	m_roleInPlaceDao->fetchAccessiblePlaces(
			places, identity.user(), AccessLevel::admin());

	if (places.size() > 1)
		throw IllegalStateException("too many places, incompatible");

	if (places.size() == 0) {
		Place place;
		createImplicitPlace(place, identity);
		places.push_back(place);
	}

	input.data().full(gateway);
	return m_gatewayDao->assignAndUpdate(gateway, places.front());
}

void GatewayService::createImplicitPlace(Place &place, Identity &identity)
{
	place.setName("Implicit");

	m_placeDao->create(place);

	RoleInPlace role;
	role.setPlace(place);
	role.setIdentity(identity);
	role.setLevel(AccessLevel::admin());

	m_roleInPlaceDao->create(role);
}

bool GatewayService::fetch(Single<Gateway> &input)
{
	return m_gatewayDao->fetch(input.target());
}

bool GatewayService::fetchFromPlace(Relation<Gateway, Place> &input)
{
	return m_gatewayDao->fetchFromPlace(input.target(), input.base());
}

void GatewayService::fetchAccessible(Relation<vector<Gateway>, User> &input)
{
	m_gatewayDao->fetchAccessible(input.target(), input.base());
}

bool GatewayService::update(SingleWithData<Gateway> &input)
{
	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	input.data().partial(gateway);
	return m_gatewayDao->update(gateway);
}

bool GatewayService::updateInPlace(RelationWithData<Gateway, Place> &input)
{
	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetchFromPlace(gateway, input.base()))
		throw NotFoundException("gateway does not exist");

	input.data().partial(gateway);

	return m_gatewayDao->update(gateway);
}

bool GatewayService::assignAndUpdate(
		RelationWithData<Gateway, Place> &input)
{
	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	if (gateway.hasPlace()) // do not leak it exists
		throw NotFoundException("gateway is already assigned");

	input.data().partial(gateway);

	return m_gatewayDao->assignAndUpdate(gateway, input.base());
}

bool GatewayService::unassign(Relation<Gateway, Place> &input)
{
	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetchFromPlace(gateway, input.base()))
		return false;

	return m_gatewayDao->unassign(gateway);
}

bool GatewayService::unassign(Relation<Gateway, User> &input)
{
	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	if (!gateway.hasPlace()) // do not leak that this gateway exists
		throw NotFoundException("gateway is not assigned");

	return m_gatewayDao->unassign(gateway);
}

void GatewayService::scanDevices(Single<Gateway> &input)
{
	m_rpc->sendListen(input.target());
}

void GatewayService::unpairDevice(Single<Gateway> &input, Device &device)
{
	m_rpc->unpairDevice(input.target(), device);
}

void GatewayService::pingGateway(Single<Gateway> &input)
{
	m_rpc->pingGateway(input.target());
}
