#include <Poco/SingletonHolder.h>

#include "dao/LocationDao.h"

using namespace BeeeOn;

LocationDao &NullLocationDao::instance()
{
	static Poco::SingletonHolder<NullLocationDao> singleton;
	return *singleton.get();
}
