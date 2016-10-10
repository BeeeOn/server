#include <Poco/Exception.h>

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

AccessLevel DefaultAccessPolicy::fetchAccessLevel(
		const ExpirableSession::Ptr session,
		const Place &place)
{
	User user(session->userID());
	return fetchAccessLevel(user, place);
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
		const ExpirableSession::Ptr session,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::guest());
}

void DefaultAccessPolicy::assureUpdate(
		const ExpirableSession::Ptr session,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::user());
}

void DefaultAccessPolicy::assureRemove(
		const ExpirableSession::Ptr session,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::admin());
}

void DefaultAccessPolicy::assureAssignGateway(
		const User &user,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(user, place), AccessLevel::admin());
}

void DefaultAccessPolicy::assureGet(
		const User &user,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(user, place), AccessLevel::guest());
}

void DefaultAccessPolicy::assureUnassign(
		const User &user,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(user, place), AccessLevel::admin());
}

void DefaultAccessPolicy::assureUpdate(
		const User &user,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(user, place), AccessLevel::user());
}

void DefaultAccessPolicy::assureScanDevices(
		const User &user,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(user, place), AccessLevel::user());
}

void DefaultAccessPolicy::assureCreateLocation(
		const ExpirableSession::Ptr session,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::user());
}

void DefaultAccessPolicy::assureGet(
		const ExpirableSession::Ptr session,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location "
				+ location.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::guest());
}

void DefaultAccessPolicy::assureUpdate(
		const ExpirableSession::Ptr session,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location "
				+ location.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::user());
}

void DefaultAccessPolicy::assureRemove(
		const ExpirableSession::Ptr session,
		const Location &location)
{
	Location tmp(location);
	if (!m_locationDao->fetch(tmp))
		throw InvalidAccessException("no such location "
				+ location.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::user());
}
