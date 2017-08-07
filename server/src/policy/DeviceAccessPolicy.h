#ifndef BEEEON_DEVICE_ACCESS_POLICY_H
#define BEEEON_DEVICE_ACCESS_POLICY_H

#include <Poco/SharedPtr.h>

#include <list>

namespace BeeeOn {

class Device;
class Gateway;
class PolicyContext;

class DeviceAccessPolicy {
public:
	typedef Poco::SharedPtr<DeviceAccessPolicy> Ptr;

	virtual ~DeviceAccessPolicy();

	virtual void assureGet(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway) = 0;

	virtual void assureGetMany(
		const PolicyContext &context,
		const std::list<Device> &devices) = 0;

	virtual void assureListActiveDevices(
		const PolicyContext &context,
		const Gateway &gateway) = 0;

	virtual void assureListInactiveDevices(
		const PolicyContext &context,
		const Gateway &gateway) = 0;

	virtual void assureUnregister(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway) = 0;

	virtual void assureActivate(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway) = 0;

	virtual void assureUpdate(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway) = 0;

	virtual void assureUpdateAndActivate(
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway)
	{
		// XXX: not optimal, to be improved later
		assureUpdate(context, device, gateway);
		assureActivate(context, device, gateway);
	}
};

}

#endif
