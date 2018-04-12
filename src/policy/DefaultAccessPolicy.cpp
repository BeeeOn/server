#include <set>
#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "model/VerifiedIdentity.h"
#include "policy/PolicyContext.h"
#include "policy/DefaultAccessPolicy.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DefaultAccessPolicy)
BEEEON_OBJECT_CASTABLE(FCMTokenAccessPolicy)
BEEEON_OBJECT_CASTABLE(GatewayAccessPolicy)
BEEEON_OBJECT_CASTABLE(IdentityAccessPolicy)
BEEEON_OBJECT_CASTABLE(LocationAccessPolicy)
BEEEON_OBJECT_CASTABLE(DeviceAccessPolicy)
BEEEON_OBJECT_CASTABLE(RoleAccessPolicy)
BEEEON_OBJECT_CASTABLE(SensorAccessPolicy)
BEEEON_OBJECT_CASTABLE(SensorHistoryAccessPolicy)
BEEEON_OBJECT_CASTABLE(ControlAccessPolicy)
BEEEON_OBJECT_PROPERTY("userDao", &DefaultAccessPolicy::setUserDao)
BEEEON_OBJECT_PROPERTY("fcmTokenDao", &DefaultAccessPolicy::setFCMTokenDao)
BEEEON_OBJECT_PROPERTY("gatewayDao", &DefaultAccessPolicy::setGatewayDao)
BEEEON_OBJECT_PROPERTY("locationDao", &DefaultAccessPolicy::setLocationDao)
BEEEON_OBJECT_PROPERTY("deviceDao", &DefaultAccessPolicy::setDeviceDao)
BEEEON_OBJECT_PROPERTY("roleInGatewayDao", &DefaultAccessPolicy::setRoleInGatewayDao)
BEEEON_OBJECT_END(BeeeOn, DefaultAccessPolicy)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DefaultAccessPolicy::DefaultAccessPolicy()
{
}

AccessLevel DefaultAccessPolicy::fetchAccessLevel(
		const PolicyContext &context,
		const Gateway &gateway)
{
	if (context.is<UserPolicyContext>()) {
		const UserPolicyContext &uc = context.cast<UserPolicyContext>();
		const AccessLevel level = m_roleInGatewayDao->fetchAccessLevel(gateway, uc.user());
		return level;
	}

	throw InvalidAccessException("unexpected policy context");
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

const User &DefaultAccessPolicy::userFromContext(const PolicyContext &context)
{
	if (context.is<UserPolicyContext>()) {
		const UserPolicyContext &uc = context.cast<UserPolicyContext>();
		return uc.user();
	}

	throw InvalidAccessException("unexpected policy context");
}

bool DefaultAccessPolicy::representsSelf(const RoleInGateway &role, const PolicyContext &self)
{
	return m_roleInGatewayDao->isUser(role, userFromContext(self));
}

bool DefaultAccessPolicy::representsSelf(const User &user, const PolicyContext &self)
{
	return user.id() == userFromContext(self).id();
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
			fetchAccessLevel(context, gateway), AccessLevel::guest());
		break;

	case GatewayAccessPolicy::ACTION_USER_UPDATE:
	case GatewayAccessPolicy::ACTION_USER_SCAN:
		assureAtLeast(
			fetchAccessLevel(context, gateway), AccessLevel::user());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

bool DefaultAccessPolicy::canSeeIdentity(const Identity &identity, const PolicyContext &self)
{
	if (self.is<UserPolicyContext>()) {
		const UserPolicyContext &uc = self.cast<UserPolicyContext>();
		return m_roleInGatewayDao->canSeeIdentity(identity, uc.user());
	}

	throw InvalidAccessException("unexpected policy context");
}

bool DefaultAccessPolicy::canSeeIdentity(const VerifiedIdentity &identity, const PolicyContext &self)
{
	if (self.is<UserPolicyContext>()) {
		const UserPolicyContext &uc = self.cast<UserPolicyContext>();
		return m_roleInGatewayDao->canSeeVerifiedIdentity(identity, uc.user());
	}

	throw InvalidAccessException("unexpected policy context");
}

void DefaultAccessPolicy::assure(
		const IdentityAccessPolicy::Action action,
		const PolicyContext &context,
		const Identity &identity)
{
	switch (action) {
	case IdentityAccessPolicy::ACTION_USER_GET:
		if (canSeeIdentity(identity, context))
			break;

		throw InvalidAccessException(
				"identity " + identity + " is inaccessible");

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
		const IdentityAccessPolicy::Action action,
		const PolicyContext &context,
		const VerifiedIdentity &identity)
{
	switch (action) {
	case IdentityAccessPolicy::ACTION_USER_GET:
		if (canSeeIdentity(identity, context))
			break;

		throw InvalidAccessException(
				"verified identity " + identity + " is inaccessible");

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
		const IdentityAccessPolicy::Action action,
		const PolicyContext &context,
		const User &user)
{
	switch (action) {
	case IdentityAccessPolicy::ACTION_USER_GET:
		if (representsSelf(user, context))
			break;

		throw InvalidAccessException(
				"user " + user + " is inaccessible");

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
			fetchAccessLevel(context, gateway), AccessLevel::user());
		break;

	case LocationAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context, gateway), AccessLevel::guest());
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
			fetchAccessLevel(context, gateway), AccessLevel::guest());
		break;

	case LocationAccessPolicy::ACTION_USER_UPDATE:
		assureAtLeast(
			fetchAccessLevel(context, gateway), AccessLevel::user());
		break;

	case LocationAccessPolicy::ACTION_USER_REMOVE:
		assureAtLeast(
			fetchAccessLevel(context, gateway), AccessLevel::user());
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
			fetchAccessLevel(context, gateway), AccessLevel::guest());
		break;

	case DeviceAccessPolicy::ACTION_USER_UNREGISTER:
	case DeviceAccessPolicy::ACTION_USER_ACTIVATE:
	case DeviceAccessPolicy::ACTION_USER_UPDATE_AND_ACTIVATE:
		assureAtLeast(
			fetchAccessLevel(context, gateway), AccessLevel::admin());
		break;

	case DeviceAccessPolicy::ACTION_USER_UPDATE:
		assureAtLeast(
			fetchAccessLevel(context, gateway), AccessLevel::user());
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
			fetchAccessLevel(context, gateway),
			AccessLevel::guest());
		break;

	case RoleAccessPolicy::ACTION_USER_INVITE:
		/**
		 * Only admin can invite others.
		 */
		assureAtLeast(
			fetchAccessLevel(context, gateway),
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
			fetchAccessLevel(context, tmp.gateway()),
			AccessLevel::guest());
		break;

	case RoleAccessPolicy::ACTION_USER_UPDATE:
		if (representsSelf(role, context))
			throw InvalidAccessException("cannot change own access level");

		assureAtLeast(
			fetchAccessLevel(context, tmp.gateway()),
			AccessLevel::admin());
		break;

	case RoleAccessPolicy::ACTION_USER_REMOVE:
		assureAtLeast(
			fetchAccessLevel(context, tmp.gateway()),
			AccessLevel::admin());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
	const SensorHistoryAccessPolicy::Action action,
	const PolicyContext &context,
	const Device &device,
	const TimeInterval &,
	const Timespan &)
{
	switch (action) {
	case SensorHistoryAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context, device.gateway()),
			AccessLevel::guest());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
	const SensorAccessPolicy::Action action,
	const PolicyContext &context,
	const Sensor &sensor,
	const Device &device)
{
	switch (action) {
	case SensorAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context, device.gateway()),
			AccessLevel::guest());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assureMany(
	const SensorAccessPolicy::Action action,
	const PolicyContext &context,
	const Device &device)
{
	switch (action) {
	case SensorAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context, device.gateway()),
			AccessLevel::guest());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
	const ControlAccessPolicy::Action action,
	const PolicyContext &context,
	const Control &control,
	const Device &device)
{
	switch (action) {
	case ControlAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context, device.gateway()),
			AccessLevel::guest());
		break;

	case ControlAccessPolicy::ACTION_USER_SET:
		assureAtLeast(
			fetchAccessLevel(context, device.gateway()),
			AccessLevel::user());
		break;
	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assureMany(
	const ControlAccessPolicy::Action action,
	const PolicyContext &context,
	const Device &device)
{
	switch (action) {
	case ControlAccessPolicy::ACTION_USER_GET:
		assureAtLeast(
			fetchAccessLevel(context, device.gateway()),
			AccessLevel::guest());
		break;

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}

void DefaultAccessPolicy::assure(
	const FCMTokenAccessPolicy::Action action,
	const PolicyContext &context,
	const FCMToken &token)
{
	FCMToken userToken(token);

	switch (action) {
	case FCMTokenAccessPolicy::ACTION_USER_REGISTER:
		if (representsSelf(token.user(), context))
			break;

		throw InvalidAccessException(
				"token " + token.id().toString() + " is inaccessible");

	case FCMTokenAccessPolicy::ACTION_USER_UNREGISTER:
		if (m_fcmTokenDao->fetch(userToken))
			if (representsSelf(token.user(), context))
				break;

		throw InvalidAccessException(
				"token " + token.id().toString() + " is inaccessible");

	default:
		throw InvalidAccessException("invalid action: " + to_string((int) action));
	}
}
