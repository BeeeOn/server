#include <sstream>

#include <Poco/Exception.h>
#include <Poco/Timestamp.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "di/Injectable.h"
#include "model/Device.h"
#include "model/ModuleInfo.h"
#include "model/ModuleValue.h"
#include "dao/poco/PocoSQLSensorHistoryDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "service/ValueConsumer.h"
#include "transaction/TransactionManager.h"
#include "util/MultiException.h"
#include "util/TimeInterval.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLSensorHistoryDao)
BEEEON_OBJECT_CASTABLE(SensorHistoryDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLSensorHistoryDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLSensorHistoryDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLSensorHistoryDao::setSQLLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLSensorHistoryDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLSensorHistoryDao)

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

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());
	unsigned int moduleID(value.module());
	int64_t timeAt = at.epochMicroseconds();
	double v = value.value();

	Statement sql = (session() << m_queryInsert(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(timeAt, "at"),
		use(v, "value")
	);

	return execute(sql) > 0;
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

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());
	unsigned int moduleID;
	int64_t timeAt = at.epochMicroseconds();
	double v;

	auto it = values.begin();
	MultiException ex;

	while (it != values.end()) {
		Statement sql = (session() << m_queryInsert(),
			use(gatewayID, "gateway_id"),
			use(deviceID, "device_id"),
			use(moduleID, "module_id"),
			use(timeAt, "at"),
			use(v, "value")
		);

		for (; it != values.end(); ++it) {
			moduleID = it->module();
			v = it->value();

			size_t result = 0;

			try {
				result = execute(sql);
			}
			catch (const Exception &e) {
				ex.caught(e);

				// reinitialize statement and continue
				// with following value
				++it;
				break;
			}

			if (result == 0)
				throw IllegalStateException(string(__func__) + " returned 0");
		}
	}

	if (!ex.empty())
		ex.rethrow();
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

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());
	unsigned int moduleID(module.id());
	unsigned long timeAt;

	Statement sql = (session() << m_queryFetch(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		into(timeAt),
		into(value)
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	at = Timestamp::fromEpochTime(timeAt);
	return true;
}

void PocoSQLSensorHistoryDao::fetchMany(
	const Device &device,
	const std::vector<ModuleInfo> &modules,
	std::vector<Nullable<ValueAt>> &values)
{
	assureHasId(device);
	assureHasId(device.gateway());
	// cannot assureHasId for module, SimpleID == 0 is allowed

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());
	unsigned int moduleID;
	unsigned long at;
	double value;

	Statement sql = (session() << m_queryFetch(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		into(at),
		into(value)
	);

	for (const auto module : modules) {
		moduleID = module.id();

		RecordSet result = executeSelect(sql);
		if (result.rowCount() > 0)
			values.emplace_back(ValueAt(at, value));
		else
			values.emplace_back(Nullable<ValueAt>());
	}
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

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());
	unsigned int moduleID(module.id());
	unsigned long start = range.start().epochTime();
	unsigned long end = range.end().epochTime();
	unsigned long intervalSeconds = interval.totalSeconds();
	unsigned long timeAt;
	double avg;
	double min;
	double max;

	Statement sql = (session() << m_queryFetchHuge(),
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
			RecordSet result = executeSelect(sql);
			if (result.rowCount() == 0)
				break;

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
