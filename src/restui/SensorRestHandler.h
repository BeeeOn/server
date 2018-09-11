#pragma once

#include <Poco/Timespan.h>

#include "l10n/Translator.h"
#include "rest/JSONRestHandler.h"
#include "service/SensorService.h"
#include "service/SensorHistoryService.h"
#include "util/TimeInterval.h"

namespace BeeeOn {

class Sensor;

namespace RestUI {

class SensorRestHandler : public JSONRestHandler {
public:
	SensorRestHandler();

	void setSensorService(SensorService::Ptr service);
	void setSensorHistoryService(SensorHistoryService::Ptr service);
	void setTranslatorFactory(TranslatorFactory::Ptr factory);

	/**
	 * @brief List all sensors related to the device given by ID.
	 *
	 * - usual responses: 200
	 * - example output (data):
	 * <pre>
	 * [
	 *   {
	 *     "id": "0",
	 *     "display_name": "Outer Temperature",
	 *     "type_id": "temperature",
	 *     "current": 21.5
	 *   },
	 *   {
	 *     "id": "1",
	 *     "display_name": "Boiler faults",
	 *     "type_id": "bitmap",
	 *     "subtype_id": "MOD_CURRENT_BOILER_OT_FAULT_FLAGS",
	 *     "current": 5
	 *   }
	 * ]
	 * </pre>
	 */
	void list(RestFlow &flow);

	/**
	 * @brief Show details of a sensor - a single module of device.
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * {
	 *   "id": "1",
	 *   "display_name": "humidity",
	 *   "type_id": "humidity",
	 *   "current": 70.2
	 * }
	 * </pre>
	 */
	void detail(RestFlow &flow);

	/**
	 * @brief Show current value of the given sensor.
	 *
	 * - usual responses: 200, 404
	 * - output is either a floating point number or null when
	 * no value for this sensor has been received
	 */
	void current(RestFlow &flow);

	/**
	 * @brief Show historical values of the given sensor.
	 * It is possible to request exact values, and aggregated
	 * values.
	 *
	 * As the amount of output data can be high from this operation,
	 * the implementation can put restrictions on some parameter
	 * combinations.
	 *
	 * Querying of history can be parametrized by:
	 *
	 * - range - can contain one of keywords: "hour", "day", "week",
	 * "month", or a number of seconds into the past, or a pair of
	 * numbers separated by comma (,) with time range in seconds
	 * - aggregation - can contain one of keywords: "avg", "min", "max"
	 * or "freq"; it is possible to give a list of keywords separated
	 * by commas ("freq" cannot be used in such list)
	 * - interval - a number of seconds representing a gap between
	 * individual values
	 *
	 * The parameter "aggregation" influences the output format. Its
	 * default value is "avg" (no avarage is usually computed for very short
	 * intervals - i.e. 1 s). Formats:
	 *
	 * The aggregation="freq" generates a summary of values for the given
	 * time range and interval. Thus, one can see how many times in a certain
	 * time interval an event has occured. E.g. between 15:00 and 16:00,
	 * the door has opened 10 times.
	 *
	 * 1. For aggregation="avg" or aggregation="min" or aggregation="max", the
	 * output format would be as follows:
	 * <pre>
	 * {
	 *   "at": <time-in-seconds>,
	 *   "value": <value-floating>
	 * }
	 * </pre>
	 * 2. For aggregation="freq", the output format would be as follows (the
	 * "value" can be a floating point number):
	 * <pre>
	 * {
	 *   "at": <time-in-seconds>,
	 *   "value": <value-floating>,
	 *   "frequency": <count-integer>
	 * }
	 * </pre>
	 * 3. For aggregation="avg,min,max" (or similar combination, the order does
	 * matter), the output format would be:
	 * {
	 *   "at": <time-in-seconds>
	 *   "values": [
	 *     <value-0-here-avg>,
	 *     <value-1-here-min>,
	 *     <value-2-here-max>,
	 *   ]
	 * }
	 *
	 * - usual responses: 200, 404
	 * - example output (data):
	 * <pre>
	 * [
	 *   {"at": 1536681651, "value": 25.6},
	 *   {"at": 1536681661, "value": 25.2},
	 *   {"at": 1536681671, "value": 25.4},
	 *   {"at": 1536681681, "value": 25.3},
	 *   {"at": 1536681691, "value": 25.3}
	 * ]
	 * </pre>
	 */
	void history(RestFlow &flow);

protected:
	void prepareDetail(RestFlow &flow, Sensor &sensor);

private:
	TimeInterval parseRange(const std::string &input) const;
	Poco::Timespan parseInterval(const std::string &input) const;
	std::vector<std::string> parseAggregation(const std::string &input) const;

private:
	SensorService::Ptr m_sensorService;
	SensorHistoryService::Ptr m_sensorHistoryService;
	TranslatorFactory::Ptr m_translatorFactory;
};

}
}
