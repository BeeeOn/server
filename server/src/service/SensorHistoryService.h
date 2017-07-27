#ifndef SENSOR_HISTORY_SERVICE_H
#define SENSOR_HISTORY_SERVICE_H

#include <vector>

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
	virtual ~SensorHistoryService();

	virtual void fetchRange(const Relation<ModuleInfo, Device> &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const std::string &aggregator,
			ValueConsumer &consumer) = 0;

};

}

#endif
