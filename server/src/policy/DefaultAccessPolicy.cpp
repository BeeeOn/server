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
	injector<DefaultAccessPolicy, RoleInPlaceDao>("roleInPlaceDao",
			&DefaultAccessPolicy::setRoleInPlaceDao);
}

AccessLevel DefaultAccessPolicy::fetchAccessLevel(
		const ExpirableSession::Ptr session,
		const Place &place)
{
	User user(session->userID());

	if (!m_userDao->fetch(user))
		throw InvalidAccessException("user does not exist");

	const AccessLevel level =
		m_roleInPlaceDao->fetchAccessLevel(place, user);

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
		const ExpirableSession::Ptr session,
		const Place &place)
{
	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::admin());
}

void DefaultAccessPolicy::assureGet(
		const ExpirableSession::Ptr session,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::guest());
}

void DefaultAccessPolicy::assureUnassign(
		const ExpirableSession::Ptr session,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::admin());
}

void DefaultAccessPolicy::assureUpdate(
		const ExpirableSession::Ptr session,
		const Gateway &gateway)
{
	Gateway tmp(gateway);
	if (!m_gatewayDao->fetch(tmp))
		throw InvalidAccessException("no such gateway "
				+ gateway.id().toString());

	const Place place(tmp.place());

	assureAtLeast(
		fetchAccessLevel(session, place),
		AccessLevel::user());
}
