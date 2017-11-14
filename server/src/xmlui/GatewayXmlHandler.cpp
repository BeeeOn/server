#include <vector>

#include <Poco/Timespan.h>

#include "di/Injectable.h"
#include "xmlui/GatewayXmlHandler.h"
#include "xmlui/XmlGatewayDeserializer.h"
#include "xmlui/Serializing.h"
#include "model/VerifiedIdentity.h"
#include "model/Gateway.h"
#include "model/LegacyGateway.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

GatewayXmlHandler::GatewayXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		Session::Ptr session,
		GatewayService &gatewayService,
		TimeZoneProvider::Ptr timeZoneProvider):
	SessionXmlHandler("gates", socket, input, session),
	m_gatewayService(gatewayService),
	m_timeZoneProvider(timeZoneProvider)
{
}

void GatewayXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");

	if (type == "getall") {
		handleGetAll();
		return;
	}

	Element *gatewayNode = root->getChildElement("gate");
	if (gatewayNode == NULL) {
		resultInvalidInput();
		return;
	}

	if (type == "register")
		handleRegister(gatewayNode);
	else if (type == "unregister")
		handleUnregister(gatewayNode);
	else if (type == "startlisten")
		handleListen(gatewayNode);
	else if (type == "get")
		handleGet(gatewayNode);
	else if (type == "update")
		handleUpdate(gatewayNode);
	else
		resultInvalidInput();
}

void GatewayXmlHandler::handleRegister(Element *gatewayNode)
{
	Gateway gateway(GatewayID::parse(gatewayNode->getAttribute("id")));
	VerifiedIdentity identity(session()->identityID());

	XmlGatewayDeserializer deserializer(*gatewayNode);
	deserializer.setTimeZoneProvider(m_timeZoneProvider);

	SingleWithData<Gateway> input(gateway, deserializer);
	User user(session()->userID());
	input.setUser(user);

	if (!m_gatewayService.registerGateway(input, identity)) {
		resultNotOwned();
		return;
	}

	resultSuccess();
}

void GatewayXmlHandler::handleUnregister(Element *gatewayNode)
{
	Gateway gateway(GatewayID::parse(gatewayNode->getAttribute("id")));
	Single<Gateway> input(gateway);
	User user(session()->userID());
	input.setUser(user);

	if (!m_gatewayService.unregister(input)) {
		resultNotOwned();
		return;
	}

	resultSuccess();
}

void GatewayXmlHandler::handleListen(Element *gatewayNode)
{
	Gateway gateway(GatewayID::parse(gatewayNode->getAttribute("id")));
	Single<Gateway> input(gateway);
	User user(session()->userID());
	input.setUser(user);

	m_gatewayService.scanDevices(input, Timespan(30, 0));
	resultSuccess();
}

void GatewayXmlHandler::handleGet(Element *gatewayNode)
{
	LegacyGateway gateway(GatewayID::parse(
			gatewayNode->getAttribute("id")));
	Single<LegacyGateway> input(gateway);
	User user(session()->userID());
	input.setUser(user);

	if (!m_gatewayService.fetch(input)) {
		resultNotFound();
		return;
	}

	resultDataStart();
	serialize(m_output, gateway);
	resultDataEnd();
}

void GatewayXmlHandler::handleUpdate(Element *gatewayNode)
{
	Gateway gateway(GatewayID::parse(gatewayNode->getAttribute("id")));

	XmlGatewayDeserializer update(*gatewayNode);
	update.setTimeZoneProvider(m_timeZoneProvider);

	SingleWithData<Gateway> input(gateway, update);
	User user(session()->userID());
	input.setUser(user);

	if (!m_gatewayService.update(input)) {
		resultNotFound();
		return;
	}

	resultSuccess();
}

void GatewayXmlHandler::handleGetAll()
{
	vector<LegacyGateway> gateways;
	User user(session()->userID());
	Relation<vector<LegacyGateway>, User> input(gateways, user);
	input.setUser(user);

	m_gatewayService.fetchAccessible(input);

	resultDataStart();
	serialize(m_output, gateways);
	resultDataEnd();
}

GatewayXmlHandlerResolver::GatewayXmlHandlerResolver():
	SessionXmlHandlerResolver("gates")
{
}

void GatewayXmlHandlerResolver::setTimeZoneProvider(TimeZoneProvider::Ptr provider)
{
	m_timeZoneProvider = provider;
}

bool GatewayXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!SessionXmlHandlerResolver::canHandle(root))
		return false;

	const string &type = root.getAttribute("type");

	if (type == "register")
		return true;
	if (type == "unregister")
		return true;
	if (type == "startlisten")
		return true;
	if (type == "get")
		return true;
	if (type == "update")
		return true;
	if (type == "getall")
		return true;

	return false;
}

XmlRequestHandler *GatewayXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	Session::Ptr session = lookupSession(*m_sessionManager, input);
	return new GatewayXmlHandler(
			socket, input, session,
			*m_gatewayService,
			m_timeZoneProvider);
}

BEEEON_OBJECT_BEGIN(BeeeOn, XmlUI, GatewayXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(SessionXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(AbstractXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(XmlRequestHandlerResolver)
BEEEON_OBJECT_REF("gatewayService", &GatewayXmlHandlerResolver::setGatewayService)
BEEEON_OBJECT_REF("sessionManager", &GatewayXmlHandlerResolver::setSessionManager)
BEEEON_OBJECT_REF("timeZoneProvider", &GatewayXmlHandlerResolver::setTimeZoneProvider)
BEEEON_OBJECT_END(BeeeOn, XmlUI, GatewayXmlHandlerResolver)
