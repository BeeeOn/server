#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "policy/DeviceAccessPolicy.h"

using namespace Poco;
using namespace BeeeOn;

void NullDeviceAccessPolicy::assureGet(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullDeviceAccessPolicy::assureUnregister(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullDeviceAccessPolicy::assureListActiveDevices(
		const PolicyContext &context,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullDeviceAccessPolicy::assureListInactiveDevices(
		const PolicyContext &context,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullDeviceAccessPolicy::assureActivate(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullDeviceAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

DeviceAccessPolicy &NullDeviceAccessPolicy::instance()
{
	static SingletonHolder<NullDeviceAccessPolicy> singleton;
	return *singleton.get();
}
