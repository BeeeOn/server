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
	injector<DefaultAccessPolicy, RoleInGatewayDao>("roleInGatewayDao",
			&DefaultAccessPolicy::setRoleInGatewayDao);
}

AccessLevel DefaultAccessPolicy::fetchAccessLevel(
		const User &user,
		const Gateway &gateway)
{
	const AccessLevel level = m_roleInGatewayDao->fetchAccessLevel(gateway, user);
	return level;
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

void DefaultAccessPolicy::assureRegister(
		const PolicyContext &context,
		const Gateway &gateway)
{
	if (m_roleInGatewayDao->isRegistered(gateway))
		throw InvalidAccessException("gateway "
				+ gateway + " is already registered");
}

void DefaultAccessPolicy::assureGet(
		const PolicyContext &context,
		const Gateway &gateway)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway), AccessLevel::guest());
}

void DefaultAccessPolicy::assureUnregister(
		const PolicyContext &context,
		const Gateway &gateway)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway), AccessLevel::guest());
}

void DefaultAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const Gateway &gateway)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway), AccessLevel::user());
}

void DefaultAccessPolicy::assureScanDevices(
		const PolicyContext &context,
		const Gateway &gateway)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway), AccessLevel::user());
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
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway), AccessLevel::guest());
}

void DefaultAccessPolicy::assureListInactiveDevices(
		const PolicyContext &context,
		const Gateway &gateway)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway), AccessLevel::admin());
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

void DefaultAccessPolicy::assureInvite(
	const PolicyContext &context,
	const Gateway &gateway,
	const AccessLevel &as)
{
	/**
	 * Only admin can invite others.
	 */
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway),
		AccessLevel::admin());
}

void DefaultAccessPolicy::assureList(
	const PolicyContext &context,
	const Gateway &gateway)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), gateway),
		AccessLevel::guest());
}

void DefaultAccessPolicy::assureRemove(
	const PolicyContext &context,
	const RoleInGateway &role)
{
	RoleInGateway tmp(role);
	if (!m_roleInGatewayDao->fetch(tmp))
		throw InvalidAccessException("no such role " + tmp);

	assureAtLeast(
		fetchAccessLevel(context.user(), tmp.gateway()),
		AccessLevel::admin());
}

void DefaultAccessPolicy::assureUpdate(
	const PolicyContext &context,
	const RoleInGateway &role)
{
	if (m_roleInGatewayDao->isUser(role, context.user()))
		throw InvalidAccessException("cannot change own access level");

	RoleInGateway tmp(role);
	if (!m_roleInGatewayDao->fetch(tmp))
		throw InvalidAccessException("no such role " + tmp);

	assureAtLeast(
		fetchAccessLevel(context.user(), tmp.gateway()),
		AccessLevel::admin());
}
