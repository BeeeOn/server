#ifndef BEEEON_SENSOR_HISTORY_ACCESS_POLICY_H
#define BEEEON_SENSOR_HISTORY_ACCESS_POLICY_H

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class PolicyContext;
class TimeInterval;
class ModuleInfo;
class Device;

class SensorHistoryAccessPolicy {
public:
	typedef Poco::SharedPtr<SensorHistoryAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
	};

	virtual ~SensorHistoryAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Device &device) = 0;
};

}

#endif
