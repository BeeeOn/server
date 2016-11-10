#include <Poco/DOM/Node.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/NodeList.h>

#include "xmlui/DeviceXmlHandler.h"
#include "xmlui/XmlDeviceDeserializer.h"
#include "xmlui/Serializing.h"
#include "model/Device.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

DeviceXmlHandler::DeviceXmlHandler(const StreamSocket &socket,
		const AutoPtr<Document> input,
		ExpirableSession::Ptr session,
		DeviceService &deviceService,
		PlaceService &placeService):
	AbstractXmlHandler("devices", socket, input, session),
	m_deviceService(deviceService),
	m_placeService(placeService)
{
}

void DeviceXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");
	const string &gateid = root->getAttribute("gateid");

	if (type == "getall") {
		handleGetAll(gateid);
		return;
	}
	else if (type == "getnew") {
		handleGetNew(gateid);
		return;
	}

	Element *deviceNode = root->getChildElement("device");
	if (deviceNode == NULL) {
		resultInvalidInput();
		return;
	}

	if (type == "get")
		handleGet(gateid, root->childNodes());
	else if (type == "unregister")
		handleUnregister(gateid, deviceNode);
	else if (type == "update")
		handleUpdate(gateid, deviceNode);
	else
		resultInvalidInput();
}

void DeviceXmlHandler::handleUnregister(const string &gateid,
		Element *deviceNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(deviceNode->getAttribute("euid")));
	Relation<Device, Gateway> input(device, gateway);
	User user(session()->userID());
	input.setUser(user);

	if (m_deviceService.unregister(input))
		resultSuccess();
	else
		resultUnexpected();
}

void DeviceXmlHandler::handleUpdate(const string &gateid,
		Element *deviceNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(deviceNode->getAttribute("euid")));
	XmlDeviceDeserializer deserializer(*deviceNode);
	RelationWithData<Device, Gateway> input(
			device, deserializer, gateway);
	User user(session()->userID());
	input.setUser(user);

	if (m_deviceService.updateAndActivate(input))
		resultSuccess();
	else
		resultUnexpected();
}

void DeviceXmlHandler::handleGet(const string &gateid,
		NodeList *nodes)
{
	Gateway gateway(GatewayID::parse(gateid));
	list<Device> devices;

	if (nodes == NULL) {
		resultInvalidInput();
		return;
	}

	for (unsigned long i = 0; i < nodes->length(); ++i) {
		const Node *node = nodes->item(i);

		if (node->nodeType() != Node::ELEMENT_NODE)
			continue;

		if (node->localName() != "device")
			continue;

		const Element *e = dynamic_cast<const Element *>(node);

		Device device(DeviceID::parse(e->getAttribute("euid")));
		devices.push_back(device);
	}

	Relation<list<Device>, Gateway> input(devices, gateway);
	User user(session()->userID());
	input.setUser(user);

	m_deviceService.fetchMany(input);

	resultDataStart();
	serialize(m_output, devices);
	resultDataEnd();
}

void DeviceXmlHandler::handleGetAll(const string &gateid)
{
	Gateway gateway(GatewayID::parse(gateid));
	vector<Device> devices;
	Relation<vector<Device>, Gateway> input(devices, gateway);
	User user(session()->userID());
	input.setUser(user);

	m_deviceService.fetchActiveBy(input);

	resultDataStart();
	serialize(m_output, devices);
	resultDataEnd();
}

void DeviceXmlHandler::handleGetNew(const string &gateid)
{
	Gateway gateway(GatewayID::parse(gateid));
	vector<Device> devices;
	Relation<vector<Device>, Gateway> input(devices, gateway);
	User user(session()->userID());
	input.setUser(user);

	m_deviceService.fetchInactiveBy(input);

	resultDataStart();
	serialize(m_output, devices);
	resultDataEnd();
}

DeviceXmlHandlerResolver::DeviceXmlHandlerResolver():
	AbstractXmlHandlerResolver("devices")
{
	injector<DeviceXmlHandlerResolver, DeviceService>(
			"deviceService",
			&DeviceXmlHandlerResolver::setDeviceService);
	injector<DeviceXmlHandlerResolver, PlaceService>(
			"placeService",
			&DeviceXmlHandlerResolver::setPlaceService);
	injector<DeviceXmlHandlerResolver, SessionManager>(
			"sessionManager",
			&DeviceXmlHandlerResolver::setSessionManager);
}

bool DeviceXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!AbstractXmlHandlerResolver::canHandle(root))
		return false;

	const string &type = root.getAttribute("type");

	if (type == "get")
		return true;
	if (type == "getall")
		return true;
	if (type == "getnew")
		return true;
	if (type == "update")
		return true;
	if (type == "unregister")
		return true;

	return false;
}

XmlRequestHandler *DeviceXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	ExpirableSession::Ptr session = lookupSession(
			*m_sessionManager, input);
	return new DeviceXmlHandler(
			socket, input, session,
			*m_deviceService, *m_placeService);
}

BEEEON_OBJECT(DeviceXmlHandlerResolver,
		BeeeOn::XmlUI::DeviceXmlHandlerResolver)
