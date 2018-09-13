#pragma once

#include <Poco/SharedPtr.h>

#include <list>

namespace BeeeOn {

class Device;
class Gateway;
class PolicyContext;

class DeviceAccessPolicy {
public:
	typedef Poco::SharedPtr<DeviceAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
		ACTION_USER_UNREGISTER,
		ACTION_USER_ACTIVATE,
		ACTION_USER_UPDATE,
		ACTION_USER_UPDATE_AND_ACTIVATE,
	};

	virtual ~DeviceAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Device &device,
		const Gateway &gateway) = 0;

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Gateway &gateway) = 0;

	virtual void assureMany(
		const Action action,
		const PolicyContext &context,
		const std::list<Device> &devices) = 0;
};

}
