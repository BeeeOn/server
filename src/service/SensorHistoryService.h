#ifndef SENSOR_HISTORY_SERVICE_H
#define SENSOR_HISTORY_SERVICE_H

#include <vector>

#include <Poco/SharedPtr.h>

#include "service/Relation.h"

namespace Poco {

class Timespan;

}

namespace BeeeOn {

class TimeInterval;
class ValueConsumer;
class ModuleInfo;
class Device;

class SensorHistoryService {
public:
	typedef Poco::SharedPtr<SensorHistoryService> Ptr;

	virtual ~SensorHistoryService();

	virtual void fetchRange(const Relation<ModuleInfo, Device> &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const std::vector<std::string> &aggregators,
			ValueConsumer &consumer) = 0;

};

}

#endif
