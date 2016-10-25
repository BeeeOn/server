#include <Poco/Exception.h>
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

void MockDeviceDao::fetchBy(std::vector<Device> &devices,
		const Gateway &gateway, bool active)
{
	for (auto pair : storage()) {
		Device::Ptr device = pair.second;

		if (device->activeSince().isNull()) {
			if (active)
				continue;
		} else {
			if (!active)
				continue;
		}

		if (device->gateway().id() == gateway.id())
			devices.push_back(*device);
	}
}

void MockDeviceDao::fetchActiveBy(vector<Device> &devices,
			const Gateway &gateway)
{
	fetchBy(devices, gateway, true);
}

void MockDeviceDao::fetchInactiveBy(vector<Device> &devices,
			const Gateway &gateway)
{
	fetchBy(devices, gateway, false);
}
