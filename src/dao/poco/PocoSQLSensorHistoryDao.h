#ifndef BEEEON_POCO_SQL_SENSOR_HISTORY_DAO_H
#define BEEEON_POCO_SQL_SENSOR_HISTORY_DAO_H

#include "dao/SensorHistoryDao.h"
#include "dao/Query.h"
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

	void setBatchSize(int size);

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

protected:
	void fetchHugeRaw(
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range,
		ValueConsumer &consumer);

	void fetchHugeAgg(
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range,
		const Poco::Timespan &interval,
		const Aggregator agg,
		ValueConsumer &consumer);

	void processFrequencies(
		ValueConsumer &consumer,
		const Poco::Data::RecordSet &result) const;
	void processSingle(
		ValueConsumer &consumer,
		const Aggregator agg,
		const Poco::Data::RecordSet &result) const;

private:
	Query m_queryInsert    {"sensors_history.insert"};
	Query m_queryFetch     {"sensors_history.fetch"};
	Query m_queryHugeRaw   {"sensors_history.huge_fetch_raw"};
	Query m_queryHugeAgg   {"sensors_history.huge_fetch_agg"};
	Query m_queryEnumAgg   {"sensors_history.huge_fetch_enum_agg"};

	int m_batchSize;
};

}

#endif
