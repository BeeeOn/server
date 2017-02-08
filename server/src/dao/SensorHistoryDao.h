#ifndef BEEEON_SENSORS_HISTORY_DAO_H
#define BEEEON_SENSORS_HISTORY_DAO_H

#include <vector>

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
	enum Aggregator {
		AGG_AVG,
		AGG_MIN,
		AGG_MAX
	};

	static bool isValid(const Aggregator agg);
	static void assureValid(const Aggregator agg);

	/**
	 * Insert the single value into database.
	 */
	virtual bool insert(
			const Device &device,
			const Poco::Timestamp &at,
			const ModuleValue &value) = 0;
	/**
	 * Insert the batch of values into database.
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
	 * Fetch values determined by the given time range.
	 */
	virtual void fetchHuge(
			const Device &device,
			const ModuleInfo &module,
			const TimeInterval &range,
			const Poco::Timespan &interval,
			const Aggregator agg,
			ValueConsumer &consumer) = 0;
};

class NullSensorHistoryDao : public SensorHistoryDao {
public:
	NullSensorHistoryDao();
	~NullSensorHistoryDao();

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
	void fetchHuge(
		const Device &device,
		const ModuleInfo &module,
		const TimeInterval &range,
		const Poco::Timespan &interval,
		const Aggregator agg,
		ValueConsumer &consumer) override;

	static SensorHistoryDao &instance();
};

}

#endif
