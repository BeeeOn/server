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

	virtual ~SensorHistoryAccessPolicy();

	virtual void assureFetchRange(
		const PolicyContext &context,
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range) = 0;
};

}

#endif
