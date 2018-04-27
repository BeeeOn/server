#include <Poco/Exception.h>

#include "dao/SensorHistoryDao.h"

using namespace Poco;
using namespace BeeeOn;

bool SensorHistoryDao::isValid(const SensorHistoryDao::Aggregator agg)
{
	switch (agg) {
	case AGG_AVG:
	case AGG_MIN:
	case AGG_MAX:
	case AGG_FREQ:
		return true;
	}

	return false;
}

void SensorHistoryDao::assureValid(const SensorHistoryDao::Aggregator agg)
{
	if (!isValid(agg))
		throw InvalidArgumentException("invalid aggregator function");
}

SensorHistoryDao::~SensorHistoryDao()
{
}
