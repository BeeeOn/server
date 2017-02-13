#include <Poco/Exception.h>
#include <Poco/Timestamp.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "model/Device.h"
#include "model/ModuleInfo.h"
#include "model/ModuleValue.h"
#include "dao/poco/PocoSQLSensorHistoryDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "service/ValueConsumer.h"
#include "util/TimeInterval.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(BeeeOn, PocoSQLSensorHistoryDao)

PocoSQLSensorHistoryDao::PocoSQLSensorHistoryDao()
{
	registerQuery(m_queryInsert);
	registerQuery(m_queryFetch);
	registerQuery(m_queryFetchHuge);
}

bool PocoSQLSensorHistoryDao::insert(
		const Device &device,
		const Timestamp &at,
		const ModuleValue &value)
{
	assureHasId(device);
	assureHasId(device.gateway());

	UInt64 deviceID(device.id());
	UInt64 gatewayID(device.gateway().id());
	unsigned int moduleID(value.module());
	unsigned long timeAt = at.epochTime();
	double v = value.value();

	Statement sql = (session() << m_queryInsert(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(timeAt, "at"),
		use(v, "value")
	);

	if (execute(sql) == 0)
		return false;

	return true;
}


void PocoSQLSensorHistoryDao::insertMany(
	const Device &device,
	const Timestamp &at,
	std::vector<ModuleValue> &values)
{
	assureHasId(device);
	assureHasId(device.gateway());

	if (values.empty())
		return;

	UInt64 deviceID(device.id());
	UInt64 gatewayID(device.gateway().id());
	unsigned int moduleID;
	unsigned long timeAt = at.epochTime();
	double v;

	Statement sql = (session() << m_queryInsert(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(timeAt, "at"),
		use(v, "value")
	);

	for (auto &value : values) {
		moduleID = value.module();
		v = value.value();

		if (execute(sql) == 0)
			throw IllegalStateException(string(__func__) + " returned 0");
	}
}

bool PocoSQLSensorHistoryDao::fetch(
	const Device &device,
	const ModuleInfo &module,
	Timestamp &at,
	double &value)
{
	assureHasId(device);
	assureHasId(device.gateway());
	// cannot assureHasId for module, SimpleID == 0 is allowed

	UInt64 deviceID(device.id());
	UInt64 gatewayID(device.gateway().id());
	unsigned int moduleID(module.id());
	unsigned long timeAt;

	Statement sql = (session() << m_queryFetch(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		into(timeAt),
		into(value)
	);

	if (execute(sql) == 0)
		return false;

	at = timeAt;
	return true;
}

void PocoSQLSensorHistoryDao::fetchHuge(
	const Device &device,
	const ModuleInfo &module,
	const TimeInterval &range,
	const Timespan &interval,
	const Aggregator agg,
	ValueConsumer &consumer)
{
	assureHasId(device);
	assureHasId(device.gateway());
	// cannot assureHasId for module, SimpleID == 0 is allowed

	assureValid(agg);

	UInt64 deviceID(device.id());
	UInt64 gatewayID(device.gateway().id());
	unsigned int moduleID(module.id());
	unsigned long start = range.start().epochTime();
	unsigned long end = range.end().epochTime();
	unsigned long intervalSeconds = interval.totalSeconds();
	unsigned long timeAt;
	double avg;
	double min;
	double max;

	Statement sql = (session() << m_queryFetchHuge(),
		use(intervalSeconds, "at_div"),
		use(intervalSeconds, "at_mul"),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(start, "start"),
		use(end, "end"),
		use(intervalSeconds, "interval"),
		into(timeAt),
		into(avg),
		into(min),
		into(max),
		Poco::Data::Keywords::range(0, 1)
	);

	consumer.begin(*module.type());

	try {
		while (!sql.done()) {
			execute(sql);

			switch (agg) {
			case AGG_AVG:
				consumer.single(ValueAt(timeAt, avg));
				break;
			case AGG_MIN:
				consumer.single(ValueAt(timeAt, min));
				break;
			case AGG_MAX:
				consumer.single(ValueAt(timeAt, max));
				break;
			}
		}
	} catch (...) {
		consumer.end();
		throw;
	}

	consumer.end();
}
