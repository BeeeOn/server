#include <Poco/SingletonHolder.h>

#include "dao/LocationDao.h"

BEEEON_OBJECT(BeeeOn_NullLocationDao, BeeeOn::NullLocationDao)

using namespace BeeeOn;

LocationDao &NullLocationDao::instance()
{
	static Poco::SingletonHolder<NullLocationDao> singleton;
	return *singleton.get();
}
