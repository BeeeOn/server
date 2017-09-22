#include <list>

#include <Poco/Exception.h>
#include <Poco/StringTokenizer.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/Sensor.h"
#include "model/User.h"
#include "model/ValueAt.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/RestValueConsumer.h"
#include "restui/SensorRestHandler.h"
#include "restui/Serializing.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, SensorRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_REF("sensorService", &SensorRestHandler::setSensorService)
BEEEON_OBJECT_REF("sensorHistoryService", &SensorRestHandler::setSensorHistoryService)
BEEEON_OBJECT_REF("translatorFactory", &SensorRestHandler::setTranslatorFactory)
BEEEON_OBJECT_END(BeeeOn, RestUI, SensorRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

SensorRestHandler::SensorRestHandler():
	JSONRestHandler("sensors")
{
	registerAction<SensorRestHandler>("list", &SensorRestHandler::list, {"gateway_id", "device_id"});
	registerAction<SensorRestHandler>("detail", &SensorRestHandler::detail, {"gateway_id", "device_id", "sensor_id"});
	registerAction<SensorRestHandler>("current", &SensorRestHandler::current, {"gateway_id", "device_id", "sensor_id"});
	registerAction<SensorRestHandler>("history", &SensorRestHandler::history, {"gateway_id", "device_id", "sensor_id"});
}

void SensorRestHandler::setSensorService(SensorService::Ptr service)
{
	m_sensorService = service;
}

void SensorRestHandler::setSensorHistoryService(SensorHistoryService::Ptr service)
{
	m_sensorHistoryService = service;
}

void SensorRestHandler::setTranslatorFactory(TranslatorFactory::Ptr factory)
{
	m_translatorFactory = factory;
}

void SensorRestHandler::list(RestFlow &flow)
{
	Translator::Ptr translator =
		m_translatorFactory->create(flow.session()->locale());

	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway(gateway);
	std::list<Sensor> sensors;
	Relation<std::list<Sensor>, Device> data(sensors, device);
	data.setUser(user);

	m_sensorService->fetchMany(data);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *translator, sensors);
	endSuccess(result);
}

void SensorRestHandler::prepareDetail(RestFlow &flow, Sensor &sensor)
{
	User user(flow.session()->userID());

	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway(gateway);
	sensor.setId(SensorID::parse(flow.param("sensor_id")));

	Relation<Sensor, Device> data(sensor, device);
	data.setUser(user);

	if (!m_sensorService->fetch(data))
		throw NotFoundException("no such sensor " + sensor);
}

void SensorRestHandler::detail(RestFlow &flow)
{
	Translator::Ptr translator =
		m_translatorFactory->create(flow.session()->locale());

	Sensor sensor;
	prepareDetail(flow, sensor);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *translator, sensor);
	endSuccess(result);
}

void SensorRestHandler::current(RestFlow &flow)
{
	Sensor sensor;
	prepareDetail(flow, sensor);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);

	if (sensor.at().isNull()) {
		result.null();
	}
	else {
		RestValueConsumer consumer(result);
		consumer.begin(*sensor.info().type());
		consumer.single(ValueAt(sensor.at().value(), sensor.value()));
		consumer.end();
	}

	endSuccess(result);
}

TimeInterval SensorRestHandler::parseRange(const string &input) const
{
	if (input == "hour")
		return TimeInterval::past(1 * Timespan::HOURS);
	if (input == "day")
		return TimeInterval::past(1 * Timespan::DAYS);
	if (input == "week")
		return TimeInterval::past(7 * Timespan::DAYS);
	if (input == "month")
		return TimeInterval::past(30 * Timespan::DAYS);

	StringTokenizer range(input, ",",
		StringTokenizer::TOK_TRIM | StringTokenizer::TOK_IGNORE_EMPTY);

	if (range.count() == 1) {
		const Timespan seconds =
			NumberParser::parseUnsigned64(range[0]) * Timespan::SECONDS;
		return TimeInterval::past(seconds);
	}
	else if (range.count() == 2) {
		const Timestamp start = Timestamp::fromEpochTime(
			NumberParser::parseUnsigned64(range[0]));
		const Timestamp end = Timestamp::fromEpochTime(
			NumberParser::parseUnsigned64(range[1]));

		return TimeInterval(start, end);
	}

	throw InvalidArgumentException("invalid range: " + input);
}

Timespan SensorRestHandler::parseInterval(const string &input) const
{
	return NumberParser::parseUnsigned(input) * Timespan::SECONDS;
}

string SensorRestHandler::parseAggregation(const string &input) const
{
	if (input == "avg")
		return input;
	if (input == "min")
		return input;
	if (input == "max")
		return input;

	throw InvalidArgumentException("invalid aggregation: " + input);
}

void SensorRestHandler::history(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway(gateway);
	ModuleInfo module;
	module.setId(ModuleInfoID::parse(flow.param("sensor_id")));
	Relation<ModuleInfo, Device> input(module, device);
	input.setUser(user);

	URI::QueryParameters queryParams = flow.uri().getQueryParameters();

	TimeInterval range = TimeInterval::past(1 * Timespan::HOURS);
	string aggregation = "avg";
	Timespan interval = 15 * Timespan::SECONDS;

	for (auto &item : queryParams) {
		if (item.first == "range")
			range = parseRange(Sanitize::common(item.second));
		else if (item.first == "aggregation")
			aggregation = parseAggregation(Sanitize::strict(item.second));
		else if (item.first == "interval")
			interval = parseInterval(Sanitize::strict(item.second));
		else
			throw InvalidArgumentException("unknown query parameter");
	}

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	result.startArray();

	RestValueConsumer consumer(result);

	try {
		m_sensorHistoryService->fetchRange(input, range, interval, aggregation, consumer);
	}
	catch (...) {
		// in case of a failure, finish the probably incomplete result
		// to at least not break the client processing, it is too late
		// to signalize any failures
		result.endArray();
		endSuccess(result);

		throw; // log the failure by upper layers
	}

	result.endArray();
	endSuccess(result);
}

