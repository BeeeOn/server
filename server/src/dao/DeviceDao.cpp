#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "dao/DeviceDao.h"

BEEEON_OBJECT(NullDeviceDao, BeeeOn::NullDeviceDao)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceDao &NullDeviceDao::instance()
{
	static SingletonHolder<NullDeviceDao> singleton;
	return *singleton.get();
}

void NullDeviceDao::fetchActiveBy(vector<Device> &devices,
			const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullDeviceDao::fetchInactiveBy(vector<Device> &devices,
			const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullDeviceDao::fetchMany(std::list<Device> &devices)
{
	throw NotImplementedException(__func__);
}
