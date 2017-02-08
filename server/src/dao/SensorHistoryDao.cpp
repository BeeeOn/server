#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "dao/SensorHistoryDao.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

bool SensorHistoryDao::isValid(const SensorHistoryDao::Aggregator agg)
{
	switch (agg) {
	case AGG_AVG:
	case AGG_MIN:
	case AGG_MAX:
		return true;
	}

	return false;
}

void SensorHistoryDao::assureValid(const SensorHistoryDao::Aggregator agg)
{
	if (!isValid(agg))
		throw InvalidArgumentException("invalid aggregator function");
}

NullSensorHistoryDao::NullSensorHistoryDao()
{
}

NullSensorHistoryDao::~NullSensorHistoryDao()
{
}

SensorHistoryDao &NullSensorHistoryDao::instance()
{
	static SingletonHolder<NullSensorHistoryDao> singleton;
	return *singleton.get();
}

bool NullSensorHistoryDao::insert(
	const Device &device,
	const Poco::Timestamp &at,
	const ModuleValue &value)
{
	throw NotImplementedException(__func__);
}

void NullSensorHistoryDao::insertMany(
	const Device &device,
	const Poco::Timestamp &at,
	vector<ModuleValue> &values)
{
	throw NotImplementedException(__func__);
}

bool NullSensorHistoryDao::fetch(
	const Device &device,
	const ModuleInfo &module,
	Timestamp &at,
	double &value)
{
	throw NotImplementedException(__func__);
}

void NullSensorHistoryDao::fetchHuge(
	const Device &device,
	const ModuleInfo &module,
	const TimeInterval &range,
	const Timespan &interval,
	const Aggregator agg,
	ValueConsumer &consumer)
{
	throw NotImplementedException(__func__);
}
