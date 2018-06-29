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

void SensorHistoryDao::assureValid(const std::vector<SensorHistoryDao::Aggregator> agg)
{
	if (agg.empty())
		throw InvalidArgumentException("no aggregation specified");
	if (agg.size() > 3)
		throw InvalidArgumentException("too many aggregations");

	bool freq = false;

	for (const auto &one : agg) {
		if (!isValid(one))
			throw InvalidArgumentException("invalid aggregator function");

		if (one == AGG_FREQ)
			freq = true;
	}

	if (freq && agg.size() > 1)
		throw InvalidArgumentException("cannot combine AGG_FREQ with others");
}

SensorHistoryDao::~SensorHistoryDao()
{
}
