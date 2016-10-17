#include <Poco/Exception.h>

#include "policy/PolicyContext.h"
#include "policy/DefaultAccessPolicy.h"

BEEEON_OBJECT(DefaultAccessPolicy, BeeeOn::DefaultAccessPolicy)

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
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), place),
		AccessLevel::guest());
}

void DefaultAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), place),
		AccessLevel::user());
}

void DefaultAccessPolicy::assureRemove(
		const PolicyContext &context,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), place),
		AccessLevel::admin());
}

void DefaultAccessPolicy::assureAssignGateway(
		const PolicyContext &context,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::admin());
}

void DefaultAccessPolicy::assureGet(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::guest());
}

void DefaultAccessPolicy::assureUnassign(
		const PolicyContext &context,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

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
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

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
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::user());
}

void DefaultAccessPolicy::assureCreateLocation(
		const PolicyContext &context,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::user());
}

void DefaultAccessPolicy::assureGet(
		const PolicyContext &context,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location "
				+ location.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::guest());
}

void DefaultAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location "
				+ location.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::user());
}

void DefaultAccessPolicy::assureRemove(
		const PolicyContext &context,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location "
				+ location.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(context.user(), place), AccessLevel::user());
}
