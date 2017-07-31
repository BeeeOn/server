#ifndef SENSOR_HISTORY_SERVICE_IMPL_H
#define SENSOR_HISTORY_SERVICE_IMPL_H

#include <vector>

#include "service/Relation.h"
#include "service/SensorHistoryService.h"
#include "transaction/Transactional.h"

namespace Poco {

class Timestamp;
class Timespan;

}

namespace BeeeOn {

class TimeInterval;
class ValueConsumer;
class ModuleInfo;
class Device;
class DeviceDao;
class SensorHistoryDao;
class SensorHistoryAccessPolicy;

class SensorHistoryServiceImpl : public SensorHistoryService, public Transactional {
public:
	SensorHistoryServiceImpl();

	void setSensorHistoryDao(SensorHistoryDao *dao);
	void setDeviceDao(DeviceDao *dao);
	void setAccessPolicy(SensorHistoryAccessPolicy *policy);

	void fetchRange(const Relation<ModuleInfo, Device> &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const std::string &aggregator,
			ValueConsumer &consumer)
	{
		BEEEON_TRANSACTION(doFetchRange(
			module, range, interval, aggregator, consumer));
	}

protected:
	void doFetchRange(const Relation<ModuleInfo, Device> &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const std::string &aggregator,
			ValueConsumer &consumer);

private:
	SensorHistoryDao *m_dao;
	DeviceDao *m_deviceDao;
	SensorHistoryAccessPolicy *m_policy;
};

}

#endif
