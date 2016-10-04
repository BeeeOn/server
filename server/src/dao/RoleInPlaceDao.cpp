#include <Poco/SingletonHolder.h>

#include "dao/RoleInPlaceDao.h"

BEEEON_OBJECT(NullRoleInPlaceDao, BeeeOn::NullRoleInPlaceDao)
BEEEON_OBJECT(MockRoleInPlaceDao, BeeeOn::MockRoleInPlaceDao)

using namespace BeeeOn;

RoleInPlaceDao &NullRoleInPlaceDao::instance()
{
	static Poco::SingletonHolder<NullRoleInPlaceDao> singleton;
	return *singleton.get();
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

		if (role.identity().user().id() != user.id())
			continue;

		return role.level();
	}

	return AccessLevel::none();
}
