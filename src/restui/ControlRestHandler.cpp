#include "restui/ControlRestHandler.h"

#include <Poco/Exception.h>
#include <Poco/JSON/PrintHandler.h>

#include "di/Injectable.h"
#include "model/Control.h"
#include "model/User.h"
#include "rest/RestFlow.h"
#include "rest/RestLinker.h"
#include "restui/ControlRestHandler.h"
#include "restui/Serializing.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RestUI, ControlRestHandler)
BEEEON_OBJECT_CASTABLE(RestHandler)
BEEEON_OBJECT_PROPERTY("controlService", &ControlRestHandler::setControlService)
BEEEON_OBJECT_PROPERTY("requestTimeout", &ControlRestHandler::setRequestTimeout)
BEEEON_OBJECT_END(BeeeOn, RestUI, ControlRestHandler)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;
using namespace BeeeOn::RestUI;

ControlRestHandler::ControlRestHandler():
	JSONRestHandler("controls"),
	m_requestTimeout(25 * Timespan::SECONDS)
{
	registerAction<ControlRestHandler>("list", &ControlRestHandler::list,
			{"gateway_id", "device_id"});
	registerAction<ControlRestHandler>("detail", &ControlRestHandler::detail,
			{"gateway_id", "device_id", "control_id"});
	registerAction<ControlRestHandler>("current", &ControlRestHandler::current,
			{"gateway_id", "device_id", "control_id"});
	registerAction<ControlRestHandler>("request_change", &ControlRestHandler::requestChange,
			{"gateway_id", "device_id", "control_id"});
}

void ControlRestHandler::setRequestTimeout(const Timespan &timeout)
{
	if (timeout.totalSeconds() < 1)
		throw InvalidArgumentException("requestTimeout must be at least a second");

	m_requestTimeout = timeout;
}

void ControlRestHandler::setControlService(ControlService::Ptr service)
{
	m_controlService = service;
}

void ControlRestHandler::list(RestFlow &flow)
{
	User user(flow.session()->userID());
	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway(gateway);
	std::list<Control> controls;
	Relation<std::list<Control>, Device> data(controls, device);
	data.setUser(user);

	m_controlService->fetchMany(data);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(), controls);
	endSuccess(result);
}

void ControlRestHandler::prepareDetail(RestFlow &flow, Control &control)
{
	User user(flow.session()->userID());

	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway(gateway);
	control.setId(ControlID::parse(flow.param("control_id")));

	Relation<Control, Device> data(control, device);
	data.setUser(user);

	if (!m_controlService->fetch(data))
		throw NotFoundException("no such control " + control);
}

void ControlRestHandler::detail(RestFlow &flow)
{
	Control control;
	prepareDetail(flow, control);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);
	serialize(result, *flow.translator(), control);
	endSuccess(result);
}

void ControlRestHandler::current(RestFlow &flow)
{
	Control control;
	prepareDetail(flow, control);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);

	result.startObject();
	serialize(result, *flow.translator(),
			control.requestedValue(),
			control.recentValue());
	result.endObject();

	endSuccess(result);
}

void ControlRestHandler::requestChange(RestFlow &flow)
{
	User user(flow.session()->userID());

	Gateway gateway(GatewayID::parse(flow.param("gateway_id")));
	Device device(DeviceID::parse(flow.param("device_id")));
	device.setGateway(gateway);
	Control control;
	control.setId(ControlID::parse(flow.param("control_id")));

	Object::Ptr input = parseInput(flow);

	if (!input->has("value"))
		throw InvalidArgumentException("missing argument value");

	const double value = input->getValue<double>("value");
	Timespan timeout = m_requestTimeout;

	if (input->has("timeout")) {
		unsigned int seconds = input->getValue<unsigned int>("timeout");
		timeout = seconds * Timespan::SECONDS;
	}

	Relation<Control, Device> data(control, device);
	data.setUser(user);

	m_controlService->requestChange(data, value, timeout);

	PrintHandler result(flow.response().stream());
	beginSuccess(result, 200);

	result.startObject();
	serialize(result, *flow.translator(),
			control.requestedValue(),
			control.recentValue());
	result.endObject();

	endSuccess(result);
}
