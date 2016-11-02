#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "model/User.h"
#include "dao/RoleInPlaceDao.h"
#include "dao/VerifiedIdentityDao.h"

BEEEON_OBJECT(NullRoleInPlaceDao, BeeeOn::NullRoleInPlaceDao)
BEEEON_OBJECT(MockRoleInPlaceDao, BeeeOn::MockRoleInPlaceDao)

using namespace std;
using namespace BeeeOn;

RoleInPlaceDao &NullRoleInPlaceDao::instance()
{
	static Poco::SingletonHolder<NullRoleInPlaceDao> singleton;
	return *singleton.get();
}

MockRoleInPlaceDao::MockRoleInPlaceDao()
{
	injector<MockRoleInPlaceDao, PlaceDao>("placeDao",
			&MockRoleInPlaceDao::setPlaceDao);
	injector<MockRoleInPlaceDao, VerifiedIdentityDao>(
		"verifiedIdentityDao",
		&MockRoleInPlaceDao::setVerifiedIdentityDao
	);
}

bool MockRoleInPlaceDao::roleRefersToUser(
		const RoleInPlace &role,
		const User &user)
{
	vector<VerifiedIdentity> identities;
	m_verifiedIdentityDao->fetchBy(identities, role.identity().email());

	for (auto identity : identities) {
		if (identity.user().id() == user.id())
			return true;
	}

	return false;
}

AccessLevel MockRoleInPlaceDao::fetchAccessLevel(
			const Place &place,
			const User &user)
{
	Iterator it = storage().begin();

	for (; it != storage().end(); ++it) {
		const RoleInPlace &role = *it->second;

		if (role.place().id() != place.id())
			continue;

		if (!roleRefersToUser(role, user))
			continue;

		return role.level();
	}

	return AccessLevel::none();
}

void MockRoleInPlaceDao::fetchAccessiblePlaces(
		std::vector<Place> &list,
		const User &user,
		const AccessLevel &atLeast)
{
	Iterator it = storage().begin();

	for (; it != storage().end(); ++it) {
		const RoleInPlace &role = *it->second;

		if (!roleRefersToUser(role, user))
			continue;

		if (role.level() >= atLeast)
			continue;

		Place place = role.place();

		if (m_placeDao->fetch(place)) {
			list.push_back(place);
		}
		else {
			throw Poco::IllegalStateException("place "
					+ place.id().toString()
					+ " not found in placeDao");
		}
	}
}

void MockRoleInPlaceDao::fetchBy(
		vector<RoleInPlace> &roles,
		const Place &place)
{
	Iterator it = storage().begin();

	for (; it != storage().end(); ++it) {
		const RoleInPlace &role = *it->second;

		if (role.place().id() != place.id())
			continue;

		roles.push_back(role);
	}
}

bool MockRoleInPlaceDao::hasUsersExcept(
		const Place &place,
		const User &user)
{
	vector<RoleInPlace> roles;
	fetchBy(roles, place);

	for (auto role : roles) {
		if (!roleRefersToUser(role, user))
			return true;
	}

	return false;
}
