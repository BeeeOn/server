#ifndef BEEEON_SENSOR_HISTORY_ACCESS_POLICY_H
#define BEEEON_SENSOR_HISTORY_ACCESS_POLICY_H

namespace BeeeOn {

class PolicyContext;
class TimeInterval;
class ModuleInfo;
class Device;

class SensorHistoryAccessPolicy {
public:
	virtual void assureFetchRange(
		const PolicyContext &context,
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range) = 0;
};

class NullSensorHistoryAccessPolicy : public SensorHistoryAccessPolicy {
public:
	NullSensorHistoryAccessPolicy();

	void assureFetchRange(
		const PolicyContext &context,
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range) override;

	static SensorHistoryAccessPolicy &instance();
};

}

#endif
