#include <Poco/DOM/Node.h>
#include <Poco/DOM/Element.h>
#include <Poco/DOM/NodeList.h>

#include "di/Injectable.h"
#include "xmlui/DeviceXmlHandler.h"
#include "xmlui/XmlDeviceDeserializer.h"
#include "xmlui/XmlDevicePropertyDeserializer.h"
#include "xmlui/Serializing.h"
#include "model/Device.h"
#include "model/DeviceProperty.h"
#include "util/CryptoConfig.h"
#include "work/Work.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;
using namespace BeeeOn::XmlUI;

DeviceXmlHandler::DeviceXmlHandler(const StreamSocket &socket,
		const AutoPtr<Document> input,
		Session::Ptr session,
		DeviceService &deviceService,
		CryptoConfig *config):
	SessionXmlHandler("devices", socket, input, session),
	m_deviceService(deviceService),
	m_config(config)
{
}

void DeviceXmlHandler::handleInputImpl()
{
	if (!requireSession())
		return;

	Element *root = m_input->documentElement();
	const string &type = root->getAttribute("type");

	if (type == "get") {
		handleGet(root->childNodes());
		return;
	}

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

	if (type == "unregister")
		handleUnregister(gateid, deviceNode);
	else if (type == "update")
		handleUpdate(gateid, deviceNode);
	else if (type == "createparameter")
		handleCreateParameter(gateid, deviceNode);
	else if (type == "updateparameter")
		handleUpdateParameter(gateid, deviceNode);
	else if (type == "deleteparameter")
		handleDeleteParameter(gateid, deviceNode);
	else if (type == "getparameter")
		handleGetParameter(gateid, deviceNode);
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

	(void) m_deviceService.unregister(input);

	resultSuccess();
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

void DeviceXmlHandler::handleGet(AutoPtr<NodeList> nodes)
{
	list<Device> devices;

	if (nodes.isNull()) {
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

		Gateway gateway(GatewayID::parse(e->getAttribute("gateid")));
		Device device(DeviceID::parse(e->getAttribute("euid")));
		device.setGateway(gateway);
		devices.push_back(device);
	}

	Single<list<Device>> input(devices);
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

void DeviceXmlHandler::handleCreateParameter(const string &gateid,
		Element *deviceNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(deviceNode->getAttribute("euid")));
	device.setGateway(gateway);

	XmlDevicePropertyDeserializer deserializer(*deviceNode, m_config);
	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(
			deviceNode->getAttribute("parameterkey")));

	RelationWithData<DeviceProperty, Device> input(property, deserializer, device);
	User user(session()->userID());
	input.setUser(user);

	if (m_deviceService.createProperty(input))
		resultSuccess();
	else
		resultUnexpected();
}

void DeviceXmlHandler::handleUpdateParameter(const string &gateid,
		Element *deviceNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(deviceNode->getAttribute("euid")));
	device.setGateway(gateway);

	XmlDevicePropertyDeserializer deserializer(*deviceNode, m_config);
	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(
			deviceNode->getAttribute("parameterkey")));

	RelationWithData<DeviceProperty, Device> input(property, deserializer, device);
	User user(session()->userID());
	input.setUser(user);

	if (m_deviceService.updateProperty(input))
		resultSuccess();
	else
		resultUnexpected();
}

void DeviceXmlHandler::handleDeleteParameter(const string &gateid,
		Element *deviceNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(deviceNode->getAttribute("euid")));
	device.setGateway(gateway);

	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(
			deviceNode->getAttribute("parameterkey")));

	switch (property.key().raw()) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
	case DevicePropertyKey::KEY_PASSWORD:
		break;
	default:
		resultInvalidInput();
		return;
	}

	Relation<const DeviceProperty, Device> input(property, device);
	User user(session()->userID());
	input.setUser(user);

	if (m_deviceService.removeProperty(input))
		resultSuccess();
	else
		resultNotFound();
}

void DeviceXmlHandler::handleGetParameter(const string &gateid,
		Element *deviceNode)
{
	Gateway gateway(GatewayID::parse(gateid));
	Device device(DeviceID::parse(deviceNode->getAttribute("euid")));
	device.setGateway(gateway);

	DeviceProperty property;
	property.setKey(DevicePropertyKey::parse(
			deviceNode->getAttribute("parameterkey")));

	Relation<DeviceProperty, Device> input(property, device);
	User user(session()->userID());
	input.setUser(user);

	if (!m_deviceService.findProperty(input)) {
		resultNotFound();
		return;
	}

	const DecryptedDeviceProperty decrypted(property, m_config);

	resultDataStart();
	serialize(m_output, decrypted, device);
	resultDataEnd();
}

DeviceXmlHandlerResolver::DeviceXmlHandlerResolver():
	SessionXmlHandlerResolver("devices")
{
}

bool DeviceXmlHandlerResolver::canHandle(
		const Element &root)
{
	if (!SessionXmlHandlerResolver::canHandle(root))
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
	if (type == "createparameter")
		return true;
	if (type == "updateparameter")
		return true;
	if (type == "deleteparameter")
		return true;
	if (type == "getparameter")
		return true;

	return false;
}

XmlRequestHandler *DeviceXmlHandlerResolver::createHandler(
		const StreamSocket &socket,
		const AutoPtr<Document> input)
{
	Session::Ptr session = lookupSession(*m_sessionManager, input);
	return new DeviceXmlHandler(
			socket, input, session, *m_deviceService, m_config);
}

BEEEON_OBJECT_BEGIN(BeeeOn, XmlUI, DeviceXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(SessionXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(AbstractXmlHandlerResolver)
BEEEON_OBJECT_CASTABLE(XmlRequestHandlerResolver)
BEEEON_OBJECT_REF("deviceService", &DeviceXmlHandlerResolver::setDeviceService)
BEEEON_OBJECT_REF("sessionManager", &DeviceXmlHandlerResolver::setSessionManager)
BEEEON_OBJECT_REF("cryptoConfig", &DeviceXmlHandlerResolver::setCryptoConfig)
BEEEON_OBJECT_END(BeeeOn, XmlUI, DeviceXmlHandlerResolver)
