#ifndef BEEEON_POCO_SQL_SENSOR_HISTORY_DAO_H
#define BEEEON_POCO_SQL_SENSOR_HISTORY_DAO_H

#include "dao/SensorHistoryDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"

namespace Poco {
namespace Data {

class RecordSet;
class Row;

}
}

namespace BeeeOn {

class PocoSQLSensorHistoryDao :
	public PocoAbstractDao,
	public SensorHistoryDao {
public:
	PocoSQLSensorHistoryDao();

	bool insert(
		const Device &device,
		const Poco::Timestamp &at,
		const ModuleValue &value) override;
	void insertMany(
		const Device &device,
		const Poco::Timestamp &at,
		std::vector<ModuleValue> &values) override;
	bool fetch(
		const Device &device,
		const ModuleInfo &module,
		Poco::Timestamp &at,
		double &value) override;
	void fetchMany(
		const Device &device,
		const std::vector<ModuleInfo> &modules,
		std::vector<Poco::Nullable<ValueAt>> &values) override;
	void fetchHuge(
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range,
		const Poco::Timespan &interval,
		const Aggregator agg,
		ValueConsumer &consumer) override;

private:
	SQLQuery m_queryInsert    {"sensors_history.insert"};
	SQLQuery m_queryFetch     {"sensors_history.fetch"};
	SQLQuery m_queryFetchHuge {"sensors_history.fetch.huge.interval"};
};

}

#endif
