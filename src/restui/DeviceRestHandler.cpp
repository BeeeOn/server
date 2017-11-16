#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/User.h"
#include "model/Gateway.h"
#include "service/GatewayService.h"
#include "service/DeviceService.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/DeviceRestHandler.h"
#include "restui/JSONDeviceDeserializer.h"
#include "restui/Serializing.h"
#include "util/JsonUtil.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, DeviceRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_REF("deviceService", &DeviceRestHandler::setDeviceService)
BEEEON_OBJECT_REF("gatewayService", &DeviceRestHandler::setGatewayService)
BEEEON_OBJECT_END(BeeeOn, RestUI, DeviceRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

DeviceRestHandler::DeviceRestHandler():
		JSONRestHandler("devices")
{
	registerAction<DeviceRestHandler>(
		"list",
		&DeviceRestHandler::list,
		{"gateway_id"}
	);
	registerAction<DeviceRestHandler>(
		"detail",
		&DeviceRestHandler::detail,
		{"gateway_id", "device_id"}
	);
	registerAction<DeviceRestHandler>(
		"update",
		&DeviceRestHandler::update,
		{"gateway_id", "device_id"}
	);
	registerAction<DeviceRestHandler>(
		"discover",
		&DeviceRestHandler::discover,
		{"gateway_id"}
	);
	registerAction<DeviceRestHandler>(
		"unpair",
		&DeviceRestHandler::unpair,
		{"gateway_id", "device_id"}
	);
}

void DeviceRestHandler::setDeviceService(SharedPtr<DeviceService> service)
{
	m_deviceService = service;
}

void DeviceRestHandler::setGatewayService(SharedPtr<GatewayService> service)
{
	m_gatewayService = service;
}

void DeviceRestHandler::list(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));

	std::vector<Device> devices;
	Relation<std::vector<Device>, Gateway> input(devices, gateway);
	input.setUser(user);

	enum FilterType {
		FILTER_NONE,
		FILTER_ACTIVE,
		FILTER_INACTIVE,
	};
	FilterType filter = FILTER_NONE;

	const URI::QueryParameters queryParams = flow.uri().getQueryParameters();
	for (auto &item : queryParams) {
		if (item.first == "filter") {
			if (item.second == "active")
				filter = FILTER_ACTIVE;
			else if (item.second == "inactive")
				filter = FILTER_INACTIVE;
			else
				throw PathSyntaxException("invalid query parameter filter: " + item.second);

			break;
		}
	}

	switch (filter) {
	case FILTER_NONE:
		m_deviceService->fetchActiveBy(input);
		m_deviceService->fetchInactiveBy(input);
		break;
	case FILTER_ACTIVE:
		m_deviceService->fetchActiveBy(input);
		break;
	case FILTER_INACTIVE:
		m_deviceService->fetchInactiveBy(input);
		break;
	}

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, devices);
	endSuccess(result);
}

void DeviceRestHandler::detail(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));
	Relation<Device, Gateway> data(device, gateway);
	data.setUser(user);

	if (!m_deviceService->fetch(data))
		throw NotFoundException("no such device " + device);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, device);
	endSuccess(result);
}

void DeviceRestHandler::update(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));

	JSONDeviceDeserializer update(parseInput(flow));
	RelationWithData<Device, Gateway> data(device, update, gateway);
	data.setUser(user);

	if (!m_deviceService->updateAndActivate(data))
		throw IllegalStateException("failed to update device " + device);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, device);
	endSuccess(result);
}

void DeviceRestHandler::discover(RestFlow &flow)
{
	Object::Ptr object;

	try {
		object = parseInput(flow);
	} catch (const JSONException &e) {
		throw SyntaxException("invalid JSON body", e);
	}

	if (!object->has("time_limit"))
		throw InvalidArgumentException("missing argument time_limit");

	const Timespan timeLimit =
		JsonUtil::extract<unsigned int>(object, "time_limit")
		* Timespan::SECONDS;

	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));

	Single<Gateway> inputGateway(gateway);
	inputGateway.setUser(user);

	const Work &work = m_gatewayService->scanDevices(inputGateway, timeLimit);
	flow.response().setStatus(202);

	const URI &location = flow.linker()
		.link("work", "detail", {work.id().toString()});

	flow.response()["Location"] = location;

	PrintHandler result(flow.response().stream());
	beginSuccess(result);

	result.startObject();
	result.key("location");
	result.value(location.toString());
	result.endObject();

	endSuccess(result);
}

void DeviceRestHandler::unpair(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));

	Relation<Device, Gateway> data(device, gateway);
	data.setUser(user);

	const Work &work = m_deviceService->unregister(data);

	flow.response().setStatus(202);

	const URI &location = flow.linker()
		.link("work", "detail", {work.id().toString()});

	flow.response()["Location"] = location;

	PrintHandler result(flow.response().stream());
	beginSuccess(result);

	result.startObject();
	result.key("location");
	result.value(location.toString());
	result.endObject();

	endSuccess(result);
}
