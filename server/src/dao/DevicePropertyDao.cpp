#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "dao/DevicePropertyDao.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

bool NullDevicePropertyDao::insert(DeviceProperty &property, const Device &device)
{
	throw NotImplementedException(__func__);
}

bool NullDevicePropertyDao::update(DeviceProperty &property, const Device &device)
{
	throw NotImplementedException(__func__);
}

bool NullDevicePropertyDao::remove(const DeviceProperty &property, const Device &device)
{
	throw NotImplementedException(__func__);
}

bool NullDevicePropertyDao::fetch(DeviceProperty &property, const Device &device)
{
	throw NotImplementedException(__func__);
}

void NullDevicePropertyDao::fetchByDevice(list<DeviceProperty> &properties, const Device &device)
{
	throw NotImplementedException(__func__);
}

DevicePropertyDao &NullDevicePropertyDao::instance()
{
	static SingletonHolder<NullDevicePropertyDao> singleton;
	return *singleton.get();
}
