#include <Poco/SingletonHolder.h>

#include "dao/RoleInPlaceDao.h"

BEEEON_OBJECT(NullRoleInPlaceDao, BeeeOn::NullRoleInPlaceDao)

using namespace std;
using namespace BeeeOn;

RoleInPlaceDao &NullRoleInPlaceDao::instance()
{
	static Poco::SingletonHolder<NullRoleInPlaceDao> singleton;
	return *singleton.get();
}
