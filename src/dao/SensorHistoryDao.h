#ifndef BEEEON_SENSORS_HISTORY_DAO_H
#define BEEEON_SENSORS_HISTORY_DAO_H

#include <vector>

#include <Poco/Nullable.h>
#include <Poco/SharedPtr.h>

#include "model/ValueAt.h"

namespace Poco {

class Timestamp;
class Timespan;

}

namespace BeeeOn {

class TimeInterval;
class ValueConsumer;
class ModuleValue;
class ModuleInfo;
class Device;

class SensorHistoryDao {
public:
	typedef Poco::SharedPtr<SensorHistoryDao> Ptr;

	enum Aggregator {
		AGG_AVG,
		AGG_MIN,
		AGG_MAX,
		AGG_FREQ,
	};

	static bool isValid(const Aggregator agg);
	static void assureValid(const Aggregator agg);
	static void assureValid(const std::vector<Aggregator> agg);

	virtual ~SensorHistoryDao();

	/**
	 * Insert the single value into database.
	 */
	virtual bool insert(
			const Device &device,
			const Poco::Timestamp &at,
			const ModuleValue &value) = 0;
	/**
	 * Insert the batch of values into database.
	 * In case of a single failed insertion, the behaviour should be either:
	 *
	 * - fail all of them
	 * - insert all possible values, avoid failing ones
	 *
	 * After the call, the vector values contains only values that were
	 * successfully inserted.
	 */
	virtual void insertMany(
			const Device &device,
			const Poco::Timestamp &at,
			std::vector<ModuleValue> &values) = 0;
	/**
	 * Fetch recent value of the given sensor and the related timestamp.
	 */
	virtual bool fetch(
			const Device &device,
			const ModuleInfo &module,
			Poco::Timestamp &at,
			double &value) = 0;

	/**
	 * Fetch recent values of the given sensors.
	 */
	virtual void fetchMany(
			const Device &device,
			const std::vector<ModuleInfo> &modules,
			std::vector<Poco::Nullable<ValueAt>> &values) = 0;

	/**
	 * Fetch values determined by the given time range.
	 */
	virtual void fetchHuge(
			const Device &device,
			const ModuleInfo &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const std::vector<Aggregator> agg,
			ValueConsumer &consumer) = 0;
};

}

#endif
