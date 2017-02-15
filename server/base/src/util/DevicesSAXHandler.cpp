#include <Poco/Logger.h>
#include <Poco/NumberParser.h>
#include <Poco/String.h>
#include <Poco/SAX/Attributes.h>

#include "util/DevicesSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

DevicesSAXHandler::DevicesSAXHandler()
{
	m_deviceExpect.insert("name");
	m_deviceExpect.insert("manufacturer");
	m_deviceExpect.insert("modules");

	m_modulesExpect.insert("sensor");
	m_modulesExpect.insert("actuator");
	m_modulesExpect.insert("refresh");
	m_modulesExpect.insert("battery");
	m_modulesExpect.insert("rssi");
	m_modulesExpect.insert("led");

	m_sensorExpect.insert("order");
	m_sensorExpect.insert("group");
	m_sensorExpect.insert("name");
	m_sensorExpect.insert("constraints");
	m_sensorExpect.insert("values");

	m_actuatorExpect.insert("order");
	m_actuatorExpect.insert("group");
	m_actuatorExpect.insert("name");
	m_actuatorExpect.insert("constraints");
	m_actuatorExpect.insert("values");
	m_actuatorExpect.insert("rules");

	m_refreshExpect.insert("default");
	m_refreshExpect.insert("constraints");
	m_refreshExpect.insert("values");

	m_constraintsExpect.insert("min");
	m_constraintsExpect.insert("max");
	m_constraintsExpect.insert("step");

	m_contentElements.insert("name");
	m_contentElements.insert("manufacturer");
	m_contentElements.insert("order");
	m_contentElements.insert("group");
	m_contentElements.insert("value");
	m_contentElements.insert("default");
	m_contentElements.insert("min");
	m_contentElements.insert("max");
	m_contentElements.insert("step");
	m_contentElements.insert("hide-module");
}

DevicesSAXHandler::~DevicesSAXHandler()
{
}

bool DevicesSAXHandler::expectElement(const SAXElement &element) const
{
	if (current().isRoot())
		return element.qName == "devices";

	if (isPathFromRoot("devices"))
		return element.qName == "device";

	if (isPathFromRoot("devices", "device"))
		return m_deviceExpect
			.find(element.qName) != m_deviceExpect.end();

	if (isPathFromRoot("devices", "device", "modules"))
		return m_modulesExpect
			.find(element.qName) != m_modulesExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "sensor"))
		return m_sensorExpect
			.find(element.qName) != m_sensorExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "actuator"))
		return m_actuatorExpect
			.find(element.qName) != m_actuatorExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "refresh"))
		return m_refreshExpect
			.find(element.qName) != m_refreshExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "*", "constraints"))
		return m_constraintsExpect
			.find(element.qName) != m_constraintsExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "*", "values"))
		return element.qName == "value";

	if (isPathFromRoot("devices", "device", "modules", "actuator", "rules"))
		return element.qName == "if";

	if (isPathFromRoot("devices", "device", "modules", "actuator", "rules", "if"))
		return element.qName == "hide-module";

	return false;
}

bool DevicesSAXHandler::expectContent(int length) const
{
	if (m_contentElements.find(current().qName) != m_contentElements.end())
		return true;

	return length == 0;
}

void DevicesSAXHandler::startElement(
		const SAXElement &element,
		const Attributes &attrList)
{
	if (logger().trace()) {
		logger().trace(string(__func__) + ": " + element.qName,
				__FILE__, __LINE__);
	}

	if (isPathFromRoot("devices", "device")) {
		XMLString id;
		if (!getAndTrimAttribute(attrList, "id", id))
			error("missing attribute id");

		if (id.empty())
			error("id value is empty");

		XMLString name;
		if (!getAndTrimAttribute(attrList, "name", name))
			error("missing attribute name");

		if (name.empty())
			error("name value is empty");

		m_device.setId(DeviceInfoID::parse(id));
		m_device.setName("");
		m_device.setDisplayName(name);
		m_device.setVendor("");
		m_device.clear();
	}

	if (isPathFromRoot("devices", "device", "modules", "*")) {
		XMLString id;
		if (!getAndTrimAttribute(attrList, "id", id))
			error("missing attribute id");

		if (id.empty())
			error("id value is empty");

		XMLString type;
		if (!getAndTrimAttribute(attrList, "type", type))
			error("missing attribute type");

		if (type.empty())
			error("type value is empty");

		XMLString unavailable;
		if (getAndTrimAttribute(attrList, "unavailable-value", unavailable)) {
			if (unavailable.empty())
				error("unavailable-value is empty");
		}

		m_module.setId(ModuleInfoID::parse(id));
		m_module.setType(new TypeInfo(TypeInfoID::parse(type)));
		m_module.setClassName(element.localName);
		m_module.setName("");
		m_module.setGroup("");
		m_module.setUnavailable(unavailable);
		m_module.setDefaultValue("");
	}
}

void DevicesSAXHandler::endElement(const SAXElement &element)
{
	if (logger().trace()) {
		logger().trace(string(__func__) + ": " + element.qName,
				__FILE__, __LINE__);
	}

	if (isPathFromRoot("devices", "device", "modules", "*", "name"))
		m_module.setName(element.content);

	if (isPathFromRoot("devices", "device", "modules", "*", "default"))
		m_module.setDefaultValue(element.content);

	if (isPathFromRoot("devices", "device", "modules", "*", "group"))
		m_module.setGroup(element.content);

	if (isPathFromRoot("devices", "device", "modules", "*")) {
		if (!m_device.add(m_module)) {
			error("module of id " + m_module
				+ " already exists in device " + m_device);
		}
	}

	if (isPathFromRoot("devices", "device", "name"))
		m_device.setName(element.content);

	if (isPathFromRoot("devices", "device", "manufacturer"))
		m_device.setVendor(element.content);

	if (isPathFromRoot("devices", "device")) {
		if (m_device.name().empty())
			error("missing name of device " + m_device);

		if (m_result.find(m_device) != m_result.end())
			error("device of id " + m_device + " already exists");

		if (logger().debug())
			logger().debug("inserting device " + m_device, __FILE__, __LINE__);

		m_result.insert(m_device);
	}
}

void DevicesSAXHandler::preprocessContent(const XMLChar c[],
		int &start, int &length)
{
	trimContent(c, start, length);
}
