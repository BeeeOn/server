#pragma once

#include <vector>

#include "dao/DeviceDao.h"
#include "dao/SensorHistoryDao.h"
#include "policy/SensorHistoryAccessPolicy.h"
#include "service/GWSSensorHistoryService.h"
#include "service/Relation.h"
#include "service/SensorHistoryService.h"
#include "transaction/Transactional.h"
#include "util/UnknownEvaluator.h"

namespace Poco {

class Timestamp;
class Timespan;

}

namespace BeeeOn {

class TimeInterval;
class ValueConsumer;
class ModuleInfo;
class Device;

class SensorHistoryServiceImpl : public SensorHistoryService, public GWSSensorHistoryService, public Transactional {
public:
	SensorHistoryServiceImpl();

	void setSensorHistoryDao(SensorHistoryDao::Ptr dao);
	void setDeviceDao(DeviceDao::Ptr dao);
	void setAccessPolicy(SensorHistoryAccessPolicy::Ptr policy);

	void fetchRange(const Relation<ModuleInfo, Device> &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const std::vector<std::string> &aggregator,
			ValueConsumer &consumer)
	{
		BEEEON_TRANSACTION(doFetchRange(
			module, range, interval, aggregator, consumer));
	}

	void insertMany(Device &device,
			const Poco::Timestamp &at,
			std::vector<ModuleValue> &values) override
	{
		return BEEEON_TRANSACTION(doInsertMany(device, at, values));
	}

	bool fetchLast(Device &device,
			ModuleInfo &module,
			Poco::Timestamp &at,
			double &value) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetchLast(device, module, at, value));
	}


protected:
	void doFetchRange(const Relation<ModuleInfo, Device> &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const std::vector<std::string> &aggregator,
			ValueConsumer &consumer);

	void doInsertMany(Device &device,
			const Poco::Timestamp &at,
			std::vector<ModuleValue> &values);

	bool doFetchLast(Device &device,
			ModuleInfo &module,
			Poco::Timestamp &at,
			double &value);

private:
	SensorHistoryDao::Ptr m_sensorHistoryDao;
	DeviceDao::Ptr m_deviceDao;
	SensorHistoryAccessPolicy::Ptr m_policy;
	UnknownEvaluator m_unknownEvaluator;
};

}
