#include <Poco/NumberParser.h>
#include <Poco/DOM/Node.h>
#include <Poco/DOM/Element.h>

#include "xmlui/SensorXmlHandler.h"
#include "xmlui/XmlValueConsumer.h"
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
		ExpirableSession::Ptr session,
		SensorHistoryService &sensorService):
	AbstractXmlHandler("devices", socket, input, session),
	m_sensorService(sensorService)
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

	resultDataStart();

	XmlValueConsumer consumer(m_output);
	m_sensorService.fetchRange(input, range, interval, aggregator, consumer);

	resultDataEnd();
}

SensorXmlHandlerResolver::SensorXmlHandlerResolver():
	AbstractXmlHandlerResolver("devices")
{
	injector("sensorHistoryService", &SensorXmlHandlerResolver::setSensorHistoryService);
	injector("sessionManager", &SensorXmlHandlerResolver::setSessionManager);
}

bool SensorXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!AbstractXmlHandlerResolver::canHandle(root))
		return false;

	const string &type = root.getAttribute("type");

	if (type == "getlog")
		return true;

	return false;
}

XmlRequestHandler *SensorXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	ExpirableSession::Ptr session = lookupSession(
			*m_sessionManager, input);
	return new SensorXmlHandler(
			socket, input, session, *m_sensorService);
}

BEEEON_OBJECT(BeeeOn, XmlUI, SensorXmlHandlerResolver)
