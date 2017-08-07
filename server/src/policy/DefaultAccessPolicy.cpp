#include <set>
#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "policy/PolicyContext.h"
#include "policy/DefaultAccessPolicy.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DefaultAccessPolicy)
BEEEON_OBJECT_CASTABLE(GatewayAccessPolicy)
BEEEON_OBJECT_CASTABLE(LocationAccessPolicy)
BEEEON_OBJECT_CASTABLE(DeviceAccessPolicy)
BEEEON_OBJECT_CASTABLE(RoleAccessPolicy)
BEEEON_OBJECT_CASTABLE(SensorHistoryAccessPolicy)
BEEEON_OBJECT_REF("userDao", &DefaultAccessPolicy::setUserDao)
BEEEON_OBJECT_REF("gatewayDao", &DefaultAccessPolicy::setGatewayDao)
BEEEON_OBJECT_REF("locationDao", &DefaultAccessPolicy::setLocationDao)
BEEEON_OBJECT_REF("deviceDao", &DefaultAccessPolicy::setDeviceDao)
BEEEON_OBJECT_REF("roleInGatewayDao", &DefaultAccessPolicy::setRoleInGatewayDao)
BEEEON_OBJECT_END(BeeeOn, DefaultAccessPolicy)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DefaultAccessPolicy::DefaultAccessPolicy()
{
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

void DefaultAccessPolicy::assure(
		const GatewayAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway)
{
	switch (action) {
	case GatewayAccessPolicy::ACTION_USER_REGISTER:
		if (m_roleInGatewayDao->isRegistered(gateway)) {
			throw InvalidAccessException("gateway "
				+ gateway + " is already registered");
		}

		break;

	case GatewayAccessPolicy::ACTION_USER_UNREGISTER:
	case GatewayAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::guest());
		break;

	case GatewayAccessPolicy::ACTION_USER_UPDATE:
	case GatewayAccessPolicy::ACTION_USER_SCAN:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::user());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
		const LocationAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway)
{
	switch (action) {
	case LocationAccessPolicy::ACTION_USER_CREATE:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::user());
		break;

	case LocationAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::guest());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
		const LocationAccessPolicy::Action action,
		const PolicyContext &context,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location " + location);

	const Gateway &gateway = tmp.gateway();

	switch (action) {
	case LocationAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::guest());
		break;

	case LocationAccessPolicy::ACTION_USER_UPDATE:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::user());
		break;

	case LocationAccessPolicy::ACTION_USER_REMOVE:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::user());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
		const DeviceAccessPolicy::Action action,
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	Device tmp(device);

	if (!m_deviceDao->fetch(tmp, gateway)) {
		throw InvalidAccessException("no such device "
				+ device + " for gateway " + gateway);
	}

	doAssure(action, context, gateway);
}

void DefaultAccessPolicy::assure(
		const DeviceAccessPolicy::Action action,
		const PolicyContext &context,
		const Gateway &gateway)
{

	doAssure(action, context, gateway);
}

void DefaultAccessPolicy::doAssure(
	const DeviceAccessPolicy::Action action,
	const PolicyContext &context,
	const Gateway &gateway)
{
	switch (action) {
	case DeviceAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::guest());
		break;

	case DeviceAccessPolicy::ACTION_USER_UNREGISTER:
	case DeviceAccessPolicy::ACTION_USER_ACTIVATE:
	case DeviceAccessPolicy::ACTION_USER_UPDATE_AND_ACTIVATE:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::admin());
		break;

	case DeviceAccessPolicy::ACTION_USER_UPDATE:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway), AccessLevel::user());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assureMany(
		const DeviceAccessPolicy::Action action,
		const PolicyContext &context,
		const list<Device> &devices)
{
	set<GatewayID> seen;

	for (auto &device : devices) {
		if (!device.hasId())
			throw InvalidAccessException(
				"no id specified for device");

		if (device.gateway().id().isNull())
			throw InvalidAccessException(
				"no id specified for gateway");

		if (seen.find(device.gateway().id()) != seen.end())
			continue;

		doAssure(action, context, device.gateway());
		seen.insert(device.gateway().id());
	}
}

void DefaultAccessPolicy::assure(
	const RoleAccessPolicy::Action action,
	const PolicyContext &context,
	const Gateway &gateway)
{
	switch (action) {
	case RoleAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway),
			AccessLevel::guest());
		break;

	case RoleAccessPolicy::ACTION_USER_INVITE:
		/**
		 * Only admin can invite others.
		 */
		assureAtLeast(
			fetchAccessLevel(context.user(), gateway),
			AccessLevel::admin());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
	const RoleAccessPolicy::Action action,
	const PolicyContext &context,
	const RoleInGateway &role)
{
	RoleInGateway tmp(role);
	if (!m_roleInGatewayDao->fetch(tmp))
		throw InvalidAccessException("no such role " + tmp);

	switch (action) {
	case RoleAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context.user(), tmp.gateway()),
			AccessLevel::guest());
		break;

	case RoleAccessPolicy::ACTION_USER_UPDATE:
		if (m_roleInGatewayDao->isUser(role, context.user()))
			throw InvalidAccessException("cannot change own access level");

		assureAtLeast(
			fetchAccessLevel(context.user(), tmp.gateway()),
			AccessLevel::admin());
		break;

	case RoleAccessPolicy::ACTION_USER_REMOVE:
		assureAtLeast(
			fetchAccessLevel(context.user(), tmp.gateway()),
			AccessLevel::admin());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assureFetchRange(
	const PolicyContext &context,
	const Device &device,
	const ModuleInfo &module,
	const TimeInterval &range)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), device.gateway()),
		AccessLevel::guest());
}
