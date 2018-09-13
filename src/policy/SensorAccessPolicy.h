#pragma once

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class Device;
class Sensor;
class PolicyContext;

class SensorAccessPolicy {
public:
	typedef Poco::SharedPtr<SensorAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
	};

	virtual ~SensorAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Sensor &sensor,
		const Device &device) = 0;

	virtual void assureMany(
		const Action action,
		const PolicyContext &context,
		const Device &device) = 0;
};

}
