#include <Poco/SingletonHolder.h>

#include "dao/DeviceDao.h"

BEEEON_OBJECT(NullDeviceDao, BeeeOn::NullDeviceDao)
BEEEON_OBJECT(MockDeviceDao, BeeeOn::MockDeviceDao)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceDao &NullDeviceDao::instance()
{
	static SingletonHolder<NullDeviceDao> singleton;
	return *singleton.get();
}
