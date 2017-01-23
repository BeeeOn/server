#include <set>
#include <Poco/Exception.h>

#include "policy/PolicyContext.h"
#include "policy/DefaultAccessPolicy.h"

BEEEON_OBJECT(DefaultAccessPolicy, BeeeOn::DefaultAccessPolicy)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DefaultAccessPolicy::DefaultAccessPolicy()
{
	injector<DefaultAccessPolicy, UserDao>("userDao",
			&DefaultAccessPolicy::setUserDao);
	injector<DefaultAccessPolicy, GatewayDao>("gatewayDao",
			&DefaultAccessPolicy::setGatewayDao);
	injector<DefaultAccessPolicy, LocationDao>("locationDao",
			&DefaultAccessPolicy::setLocationDao);
	injector<DefaultAccessPolicy, DeviceDao>("deviceDao",
			&DefaultAccessPolicy::setDeviceDao);
	injector<DefaultAccessPolicy, RoleInPlaceDao>("roleInPlaceDao",
			&DefaultAccessPolicy::setRoleInPlaceDao);
}

AccessLevel DefaultAccessPolicy::fetchAccessLevel(
		const User &user,
		const Place &place)
{
	User tmp(user);

	if (!m_userDao->fetch(tmp))
		throw InvalidAccessException("user does not exist");

	const AccessLevel level =
		m_roleInPlaceDao->fetchAccessLevel(place, tmp);

	return level;
}

AccessLevel DefaultAccessPolicy::fetchAccessLevel(
		const User &user,
		const Gateway &gateway)
{
	Gateway tmp(gateway);

	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("gateway does not exist");

	return fetchAccessLevel(user, tmp.place());
}

void DefaultAccessPolicy::assureAtLeast(
		const AccessLevel &current,
		const AccessLevel &required)
{
	if (current > required) {
		throw InvalidAccessException("at least "
				+ required.toString()
				+ " level is required ("
				+ current.toString()
				+ ")");
	}
}

void DefaultAccessPolicy::assureGet(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway " + gateway);

	if (!tmp.hasPlace())
		throw InvalidAccessException("gateways has no place");

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::guest());
}

void DefaultAccessPolicy::assureUnregister(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway " + gateway);

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::admin());
}

void DefaultAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway " + gateway);

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::user());
}

void DefaultAccessPolicy::assureScanDevices(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway " + gateway);

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::user());
}

void DefaultAccessPolicy::assureCreateLocation(
		const PolicyContext &context,
		const Gateway &gateway)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway), AccessLevel::user());
}

void DefaultAccessPolicy::assureGet(
		const PolicyContext &context,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location " + location);

	assureAtLeast(
		fetchAccessLevel(context.user(), tmp.gateway()), AccessLevel::guest());
}

void DefaultAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location " + location);

	assureAtLeast(
		fetchAccessLevel(context.user(), tmp.gateway()), AccessLevel::user());
}

void DefaultAccessPolicy::assureRemove(
		const PolicyContext &context,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location " + location);

	assureAtLeast(
		fetchAccessLevel(context.user(), tmp.gateway()), AccessLevel::user());
}

void DefaultAccessPolicy::assureGet(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	Device tmp(device);

	if (!m_deviceDao->fetch(tmp, gateway)) {
		throw InvalidAccessException("no such device "
				+ device + " for gateway " + gateway);
	}

	assureAtLeast(fetchAccessLevel(context.user(), gateway),
			AccessLevel::guest());
}

void DefaultAccessPolicy::assureGetMany(
		const PolicyContext &context,
		const list<Device> &devices)
{
	set<GatewayID> seen;

	for (auto &device : devices) {
		const GatewayID &id = device.gateway().id();

		if (!device.hasId())
			throw InvalidAccessException(
				"no id specified for device");

		if (id.isNull())
			throw InvalidAccessException(
				"no id specified for gateway");

		if (seen.find(id) != seen.end())
			continue;

		Gateway gateway(id);
		assureAtLeast(fetchAccessLevel(context.user(), gateway),
				AccessLevel::guest());

		seen.insert(id);
	}
}

void DefaultAccessPolicy::assureListActiveDevices(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway " + gateway);

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::guest());
}

void DefaultAccessPolicy::assureListInactiveDevices(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway " + gateway);

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::admin());
}

void DefaultAccessPolicy::assureUnregister(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	Device tmp(device);

	if (!m_deviceDao->fetch(tmp, gateway)) {
		throw InvalidAccessException("no such device "
				+ device + " for gateway " + gateway);
	}

	assureAtLeast(fetchAccessLevel(context.user(), gateway),
			AccessLevel::admin());
}

void DefaultAccessPolicy::assureActivate(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	Device tmp(device);

	if (!m_deviceDao->fetch(tmp, gateway)) {
		throw InvalidAccessException("no such device "
				+ device + " for gateway " + gateway);
	}

	assureAtLeast(fetchAccessLevel(context.user(), gateway),
			AccessLevel::admin());
}

void DefaultAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	Device tmp(device);

	if (!m_deviceDao->fetch(tmp, gateway)) {
		throw InvalidAccessException("no such device "
				+ device + " for gateway " + gateway);
	}

	assureAtLeast(fetchAccessLevel(context.user(), gateway),
			AccessLevel::user());
}
