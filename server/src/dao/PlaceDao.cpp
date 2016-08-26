#include <Poco/SingletonHolder.h>

#include "dao/PlaceDao.h"

BEEEON_OBJECT(MockPlaceDao, BeeeOn::MockPlaceDao)
BEEEON_OBJECT(NullPlaceDao, BeeeOn::NullPlaceDao)

using namespace BeeeOn;

PlaceDao &NullPlaceDao::instance()
{
	static Poco::SingletonHolder<NullPlaceDao> singleton;
	return *singleton.get();
}
