#include <vector>

#include "xmlui/GatewayXmlHandler.h"
#include "xmlui/XmlGatewayDeserializer.h"
#include "xmlui/Serializing.h"
#include "model/VerifiedIdentity.h"
#include "model/Gateway.h"
#include "model/Place.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

GatewayXmlHandler::GatewayXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		ExpirableSession::Ptr session,
		GatewayService &gatewayService,
		PlaceService &placeService,
		GatewayAccessPolicy &policy):
	AbstractXmlHandler("gates", socket, input, session),
	m_gatewayService(gatewayService),
	m_placeService(placeService),
	m_accessPolicy(policy)
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
	SingleWithData<Gateway> input(gateway, deserializer);
	User user(session()->userID());

	// approved to everybody, such gateway is associated with a
	// place where the user is admin or to a new implicit place
	if (!m_gatewayService.registerGateway(input, identity)) {
		resultNotOwned();
		return;
	}

	resultSuccess();
}

void GatewayXmlHandler::handleUnregister(Element *gatewayNode)
{
	Gateway gateway(GatewayID::parse(gatewayNode->getAttribute("id")));
	User user(session()->userID());

	m_accessPolicy.assureUnassign(user, gateway);

	if (!m_gatewayService.unassign(gateway, user)) {
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

	m_accessPolicy.assureScanDevices(user, gateway);

	m_gatewayService.scanDevices(input);
	resultSuccess();
}

void GatewayXmlHandler::handleGet(Element *gatewayNode)
{
	Gateway gateway(GatewayID::parse(gatewayNode->getAttribute("id")));
	Single<Gateway> input(gateway);
	User user(session()->userID());

	m_accessPolicy.assureGet(user, gateway);

	if (!m_gatewayService.fetch(input)) {
		resultNotFound();
		return;
	}

	if (!gateway.hasPlace()) {
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
	SingleWithData<Gateway> input(gateway, update);
	User user(session()->userID());

	m_accessPolicy.assureUpdate(user, gateway);

	if (!m_gatewayService.update(input)) {
		resultNotFound();
		return;
	}

	resultSuccess();
}

void GatewayXmlHandler::handleGetAll()
{
	vector<Gateway> gateways;
	User user(session()->userID());

	m_gatewayService.fetchAccessible(gateways, user);

	resultDataStart();
	serialize(m_output, gateways);
	resultDataEnd();
}

GatewayXmlHandlerResolver::GatewayXmlHandlerResolver():
	AbstractXmlHandlerResolver("gates")
{
	injector<GatewayXmlHandlerResolver, GatewayService>(
			"gatewayService",
			&GatewayXmlHandlerResolver::setGatewayService);
	injector<GatewayXmlHandlerResolver, PlaceService>(
			"placeService",
			&GatewayXmlHandlerResolver::setPlaceService);
	injector<GatewayXmlHandlerResolver, GatewayAccessPolicy>(
			"accessPolicy",
			&GatewayXmlHandlerResolver::setAccessPolicy);
	injector<GatewayXmlHandlerResolver, SessionManager>(
			"sessionManager",
			&GatewayXmlHandlerResolver::setSessionManager);
}

bool GatewayXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!AbstractXmlHandlerResolver::canHandle(root))
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
	ExpirableSession::Ptr session = lookupSession(
			*m_sessionManager, input);
	return new GatewayXmlHandler(
			socket, input, session,
			*m_gatewayService, *m_placeService,
			*m_accessPolicy);
}

BEEEON_OBJECT(GatewayXmlHandlerResolver,
		BeeeOn::XmlUI::GatewayXmlHandlerResolver)
