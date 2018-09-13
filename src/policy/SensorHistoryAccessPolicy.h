#pragma once

#include "util/TimeInterval.h"

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

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
		const Device &device,
		const TimeInterval &range,
		const Poco::Timespan &interval) = 0;
};

}
