#include <Poco/NumberParser.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/Element.h>

#include "di/Injectable.h"
#include "xmlui/SensorXmlHandler.h"
#include "xmlui/XmlValueConsumer.h"
#include "model/Control.h"
#include "model/Device.h"
#include "model/ModuleInfo.h"
#include "service/Relation.h"
#include "service/SensorHistoryService.h"
#include "util/TimeInterval.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

SensorXmlHandler::SensorXmlHandler(const StreamSocket &socket,
		const AutoPtr<Document> input,
		Session::Ptr session,
		ControlService &controlService,
		SensorHistoryService &sensorService,
		const Timespan &setStateTimeout):
	SessionXmlHandler("devices", socket, input, session),
	m_controlService(controlService),
	m_sensorService(sensorService),
	m_setStateTimeout(setStateTimeout)
{
}

void SensorXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");
	const string &gateid = root->getAttribute("gateid");

	if (type == "getlog") {
		Element *logsNode = root->getChildElement("logs");
		handleGetLog(gateid, logsNode);
		return;
	}
	else if (type == "setstate") {
		Element *deviceNode = root->getChildElement("device");
		handleSetState(gateid, deviceNode);
		return;
	}

	resultInvalidInput();
}

void SensorXmlHandler::handleGetLog(const string &gateid,
		Element *logsNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(logsNode->getAttribute("deviceeuid")));
	device.setGateway(gateway);
	ModuleInfo module;
	module.setId(ModuleInfoID::parse(logsNode->getAttribute("moduleid")));

	Timestamp start(NumberParser::parseUnsigned64(
			logsNode->getAttribute("from")) * 1000000);
	Timestamp end(NumberParser::parseUnsigned64(
			logsNode->getAttribute("to")) * 1000000);
	TimeInterval range(start, end);

	Timespan interval(
		NumberParser::parseUnsigned(logsNode->getAttribute("interval")),
		0
	);

	const string aggregator(logsNode->getAttribute("ftype"));

	Relation<ModuleInfo, Device> input(module, device);
	User user(session()->userID());
	input.setUser(user);

	XmlValueConsumer consumer(m_output);

	try {
		m_sensorService.fetchRange(input, range, interval, {aggregator}, consumer);
	}
	BEEEON_CATCH_CHAIN_ACTION(logger(),
		if (consumer.hasBegin()) {
			// in case of a failure, finish the probably incomplete result
			// to at least not break the client processing, it is too late
			// to signalize any failures
			consumer.end();
		}
		else {
			throw; // handle as error
		}
	)
}

void SensorXmlHandler::handleSetState(const string &gateid,
		Element *deviceNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(deviceNode->getAttribute("euid")));
	device.setGateway(gateway);
	Control control;
	control.setId(ControlID::parse(deviceNode->getAttribute("moduleid")));

	const double value = NumberParser::parseFloat(deviceNode->getAttribute("value"));

	Relation<Control, Device> input(control, device);
	User user(session()->userID());
	input.setUser(user);

	m_controlService.requestChange(input, value, m_setStateTimeout, false);
	resultSuccess();
}

SensorXmlHandlerResolver::SensorXmlHandlerResolver():
	SessionXmlHandlerResolver("devices"),
	m_setStateTimeout(25 * Timespan::SECONDS)
{
}

void SensorXmlHandlerResolver::setControlService(
		ControlService::Ptr service)
{
	m_controlService = service;
}

void SensorXmlHandlerResolver::setSensorHistoryService(
		SensorHistoryService::Ptr service)
{
	m_sensorService = service;
}

void SensorXmlHandlerResolver::setSetStateTimeout(const Timespan &timeout)
{
	m_setStateTimeout = timeout;
}

bool SensorXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!SessionXmlHandlerResolver::canHandle(root))
		return false;

	const string &type = root.getAttribute("type");

	if (type == "getlog")
		return true;
	if (type == "setstate")
		return true;

	return false;
}

XmlRequestHandler *SensorXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	Session::Ptr session = lookupSession(
			*m_sessionManager, input);
	return new SensorXmlHandler(
			socket, input, session,
			*m_controlService,
			*m_sensorService,
			m_setStateTimeout);
}

BEEEON_OBJECT_BEGIN(BeeeOn, XmlUI, SensorXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(SessionXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(AbstractXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(XmlRequestHandlerResolver)
BEEEON_OBJECT_PROPERTY("controlService", &SensorXmlHandlerResolver::setControlService)
BEEEON_OBJECT_PROPERTY("sensorHistoryService", &SensorXmlHandlerResolver::setSensorHistoryService)
BEEEON_OBJECT_PROPERTY("sessionManager", &SensorXmlHandlerResolver::setSessionManager)
BEEEON_OBJECT_PROPERTY("setStateTimeout", &SensorXmlHandlerResolver::setSetStateTimeout)
BEEEON_OBJECT_END(BeeeOn, XmlUI, SensorXmlHandlerResolver)
