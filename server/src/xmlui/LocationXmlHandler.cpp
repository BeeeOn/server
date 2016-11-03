#include "xmlui/LocationXmlHandler.h"
#include "xmlui/XmlLocationDeserializer.h"
#include "xmlui/Serializing.h"
#include "model/Location.h"
#include "model/Place.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

LocationXmlHandler::LocationXmlHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input,
		ExpirableSession::Ptr session,
		LocationService &locationService):
	AbstractXmlHandler("locations", socket, input, session),
	m_locationService(locationService)
{
}

void LocationXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");
	const string &gateid = root->getAttribute("gateid");

	if (gateid.empty()) {
		resultInvalidInput();
		return;
	}

	if (type == "getall") {
		handleGetAll(gateid);
		return;
	}

	Element *locationNode = root->getChildElement("location");
	if (locationNode == NULL) {
		resultInvalidInput();
		return;
	}

	if (type == "add")
		handleAdd(gateid, locationNode);
	else if (type == "delete")
		handleDelete(gateid, locationNode);
	else if (type == "update")
		handleUpdate(gateid, locationNode);
	else
		resultInvalidInput();
}

void LocationXmlHandler::handleAdd(
		const string &gateid,
		Element *locationNode)
{
	Gateway gateway(GatewayID::parse(gateid));

	Location location;
	XmlLocationDeserializer deserializer(*locationNode);
	RelationWithData<Location, Gateway> input(
			location, deserializer, gateway);
	User user(session()->userID());
	input.setUser(user);
	
	m_locationService.createIn(input);

	resultDataStart();
	serialize(m_output, location);
	resultDataEnd();
}

void LocationXmlHandler::handleDelete(
		const string &gateid,
		Element *locationNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Location location(LocationID::parse(
			locationNode->getAttribute("locationid")));
	Relation<Location, Gateway> input(location, gateway);
	User user(session()->userID());
	input.setUser(user);

	if (!m_locationService.removeFrom(input)) {
		resultNotFound();
		return;
	}

	resultSuccess();
}

void LocationXmlHandler::handleUpdate(
		const string &gateid,
		Element *locationNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Location location(LocationID::parse(
			locationNode->getAttribute("locationid")));
	XmlLocationDeserializer update(*locationNode);
	RelationWithData<Location, Gateway> input(
			location, update, gateway);
	User user(session()->userID());
	input.setUser(user);

	if (!m_locationService.updateIn(input)) {
		resultNotFound();
		return;
	}

	resultSuccess();
}

void LocationXmlHandler::handleGetAll(const string &gateid)
{
	Gateway gateway(GatewayID::parse(gateid));
	vector<Location> locations;
	Relation<vector<Location>, Gateway> input(locations, gateway);
	User user(session()->userID());
	input.setUser(user);

	m_locationService.fetchBy(input);

	resultDataStart();
	serialize(m_output, locations);
	resultDataEnd();
}

LocationXmlHandlerResolver::LocationXmlHandlerResolver():
	AbstractXmlHandlerResolver("locations")
{
	injector<LocationXmlHandlerResolver, LocationService>(
			"locationService",
			&LocationXmlHandlerResolver::setLocationService);
	injector<LocationXmlHandlerResolver, SessionManager>(
			"sessionManager",
			&LocationXmlHandlerResolver::setSessionManager);
}

bool LocationXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!AbstractXmlHandlerResolver::canHandle(root))
		return false;

	if (root.getAttribute("type") == "add")
		return true;
	if (root.getAttribute("type") == "delete")
		return true;
	if (root.getAttribute("type") == "update")
		return true;
	if (root.getAttribute("type") == "getall")
		return true;

	return false;
}

XmlRequestHandler *LocationXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	ExpirableSession::Ptr session = lookupSession(
			*m_sessionManager, input);
	return new LocationXmlHandler(
			socket, input, session, *m_locationService);
}

BEEEON_OBJECT(LocationXmlHandlerResolver,
		BeeeOn::XmlUI::LocationXmlHandlerResolver)
