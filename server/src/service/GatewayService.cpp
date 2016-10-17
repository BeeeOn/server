#include <vector>
#include <Poco/Exception.h>

#include "service/GatewayService.h"
#include "server/AccessLevel.h"

BEEEON_OBJECT(GatewayService, BeeeOn::GatewayService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GatewayService::GatewayService():
	m_gatewayDao(&NullGatewayDao::instance()),
	m_roleInPlaceDao(&NullRoleInPlaceDao::instance()),
	m_placeDao(&NullPlaceDao::instance()),
	m_identityDao(&NullIdentityDao::instance()),
	m_rpc(&NullGatewayRPC::instance())
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

bool GatewayService::fetchFromPlace(Gateway &gateway, const Place &place)
{
	return m_gatewayDao->fetchFromPlace(gateway, place);
}

void GatewayService::fetchAccessible(vector<Gateway> &gateways,
		const User &user)
{
	m_gatewayDao->fetchAccessible(gateways, user);
}

bool GatewayService::update(SingleWithData<Gateway> &input)
{
	Gateway &gateway = input.target();

	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	input.data().partial(gateway);
	return m_gatewayDao->update(gateway);
}

bool GatewayService::updateInPlace(Gateway &gateway,
		const Deserializer<Gateway> &update,
		const Place &place)
{
	if (!m_gatewayDao->fetchFromPlace(gateway, place))
		throw NotFoundException("gateway does not exist");

	update.partial(gateway);

	return m_gatewayDao->update(gateway);
}

bool GatewayService::assignAndUpdate(Gateway &gateway,
		const Deserializer<Gateway> &update,
		const Place &place)
{
	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	if (gateway.hasPlace()) // do not leak it exists
		throw NotFoundException("gateway is already assigned");

	update.partial(gateway);

	return m_gatewayDao->assignAndUpdate(gateway, place);
}

bool GatewayService::unassign(Gateway &gateway, const Place &place)
{
	if (!m_gatewayDao->fetchFromPlace(gateway, place))
		return false;

	return m_gatewayDao->unassign(gateway);
}

bool GatewayService::unassign(Gateway &gateway, const User &user)
{
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
