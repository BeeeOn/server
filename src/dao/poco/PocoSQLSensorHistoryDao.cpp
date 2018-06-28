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
BEEEON_OBJECT_PROPERTY("batchSize", &PocoSQLSensorHistoryDao::setBatchSize)
BEEEON_OBJECT_PROPERTY("daoManager", &PocoSQLSensorHistoryDao::setDaoManager)
BEEEON_OBJECT_PROPERTY("transactionManager", &PocoSQLSensorHistoryDao::setTransactionManager)
BEEEON_OBJECT_PROPERTY("sqlLoader", &PocoSQLSensorHistoryDao::setQueryLoader)
BEEEON_OBJECT_PROPERTY("profiler", &PocoSQLSensorHistoryDao::setQueryProfiler)
BEEEON_OBJECT_HOOK("done", &PocoSQLSensorHistoryDao::loadQueries)
BEEEON_OBJECT_HOOK("cleanup", &PocoSQLSensorHistoryDao::clearQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLSensorHistoryDao)

PocoSQLSensorHistoryDao::PocoSQLSensorHistoryDao():
	m_batchSize(1024)
{
	registerQuery(m_queryInsert);
	registerQuery(m_queryFetch);
	registerQuery(m_queryHugeRaw);
	registerQuery(m_queryHugeAgg);
	registerQuery(m_queryEnumAgg);
}

void PocoSQLSensorHistoryDao::setBatchSize(int size)
{
	if (size < 1)
		throw InvalidArgumentException("batchSize must be at least 1");

	m_batchSize = size;
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
	Nullable<double> v;

	if (value.isValid())
		v = value.value();

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
	Nullable<double> v;

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

			if (it->isValid())
				v = it->value();
			else
				v.clear();

			size_t result = 0;

			try {
				result = execute(sql);
			}
			catch (const Exception &e) {
				ex.caught(e);

				// reinitialize statement and continue
				// with following value
				it = values.erase(it);
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

	Statement sql = (session() << m_queryFetch(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	auto &row = result.row(0);

	at = Timestamp::fromEpochTime(row.get(0).convert<unsigned long>());
	if (row.get(1).isEmpty())
		value = std::nan("");
	else
		value = row.get(1).convert<double>();

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

	Statement sql = (session() << m_queryFetch(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id")
	);

	for (const auto module : modules) {
		moduleID = module.id();

		RecordSet result = executeSelect(sql);
		if (result.rowCount() > 0) {
			auto &row = result.row(0);
			const unsigned long at = row.get(0).convert<unsigned long>();

			if (row.get(1).isEmpty())
				values.emplace_back(ValueAt(at, NAN));
			else
				values.emplace_back(ValueAt(at, row.get(1).convert<double>()));
		}
		else {
			values.emplace_back(Nullable<ValueAt>());
		}
	}
}

void PocoSQLSensorHistoryDao::fetchHugeRaw(
	const Device &device,
	const ModuleInfo &module,
	const TimeInterval &range,
	ValueConsumer &consumer)
{
	assureHasId(device);
	assureHasId(device.gateway());
	// cannot assureHasId for module, SimpleID == 0 is allowed

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());
	unsigned int moduleID(module.id());
	unsigned long start = range.start().epochTime();
	unsigned long end = range.end().epochTime();

	Statement sql = (session() << m_queryHugeRaw(),
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(start, "start"),
		use(end, "end"),
		Poco::Data::Keywords::range(0, m_batchSize)
	);

	consumer.begin(*module.type());

	try {
		while (!sql.done()) {
			RecordSet result = executeSelect(sql);
			if (result.rowCount() == 0)
				break;

			for (auto &row : result) {
				const unsigned long timeAt = row.get(0).convert<unsigned long>();
				double value = NAN;

				if (!row.get(1).isEmpty())
					value = row.get(1).convert<double>();

				consumer.single(ValueAt(timeAt, value));
			}
		}
	} catch (...) {
		consumer.end();
		throw;
	}

	consumer.end();
}

void PocoSQLSensorHistoryDao::fetchHugeAgg(
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

	const auto &query = agg == AGG_FREQ ?
		m_queryEnumAgg() : m_queryHugeAgg();

	Statement sql = (session() << query,
		use(gatewayID, "gateway_id"),
		use(deviceID, "device_id"),
		use(moduleID, "module_id"),
		use(start, "start"),
		use(end, "end"),
		use(intervalSeconds, "interval"),
		Poco::Data::Keywords::range(0, m_batchSize)
	);

	consumer.begin(*module.type());

	try {
		while (!sql.done()) {
			RecordSet result = executeSelect(sql);
			if (result.rowCount() == 0)
				break;

			for (auto &row : result) {
				const unsigned long timeAt = row.get(0).convert<unsigned long>();
				switch (agg) {
				case AGG_AVG:
					consumer.single(ValueAt(
						timeAt, row.get(1).convert<double>()));
					break;
				case AGG_MIN:
					consumer.single(ValueAt(
						timeAt, row.get(2).convert<double>()));
					break;
				case AGG_MAX:
					consumer.single(ValueAt(
						timeAt, row.get(3).convert<double>()));
					break;
				case AGG_FREQ:
					consumer.frequency(
						ValueAt(timeAt, row.get(1).convert<double>()),
						row.get(2).convert<size_t>());
					break;
				}
			}
		}
	} catch (...) {
		consumer.end();
		throw;
	}

	consumer.end();
}

void PocoSQLSensorHistoryDao::fetchHuge(
	const Device &device,
	const ModuleInfo &module,
	const TimeInterval &range,
	const Timespan &interval,
	const Aggregator agg,
	ValueConsumer &consumer)
{
	if (interval <= 5 * Timespan::SECONDS) {
		fetchHugeRaw(device, module, range, consumer);
	}
	else {
		fetchHugeAgg(device, module, range,
				interval, agg, consumer);
	}
}
