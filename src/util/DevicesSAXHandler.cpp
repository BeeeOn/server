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
	m_deviceExpect.insert("vendor");
	m_deviceExpect.insert("match");
	m_deviceExpect.insert("modules");

	m_matchExpect.insert("exact");
	m_matchExpect.insert("pattern");

	m_modulesExpect.insert("sensor");
	m_modulesExpect.insert("actuator");
	m_modulesExpect.insert("control");
	m_modulesExpect.insert("refresh");
	m_modulesExpect.insert("battery");
	m_modulesExpect.insert("rssi");
	m_modulesExpect.insert("led");

	m_sensorExpect.insert("order");
	m_sensorExpect.insert("group");
	m_sensorExpect.insert("name");
	m_sensorExpect.insert("constraints");
	m_sensorExpect.insert("values");
	m_sensorExpect.insert("unknown");

	m_controlExpect.insert("order");
	m_controlExpect.insert("group");
	m_controlExpect.insert("name");
	m_controlExpect.insert("constraints");
	m_controlExpect.insert("values");
	m_controlExpect.insert("rules");
	m_controlExpect.insert("unknown");

	m_refreshExpect.insert("constraints");
	m_refreshExpect.insert("values");

	m_constraintsExpect.insert("min");
	m_constraintsExpect.insert("max");
	m_constraintsExpect.insert("step");

	m_contentElements.insert("name");
	m_contentElements.insert("vendor");
	m_contentElements.insert("order");
	m_contentElements.insert("group");
	m_contentElements.insert("value");
	m_contentElements.insert("min");
	m_contentElements.insert("max");
	m_contentElements.insert("step");
	m_contentElements.insert("hide-module");
	m_contentElements.insert("exact");
	m_contentElements.insert("pattern");
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

	if (isPathFromRoot("devices", "device", "match"))
		return m_matchExpect
			.find(element.qName) != m_matchExpect.end();

	if (isPathFromRoot("devices", "device", "modules"))
		return m_modulesExpect
			.find(element.qName) != m_modulesExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "sensor"))
		return m_sensorExpect
			.find(element.qName) != m_sensorExpect.end();

	// obsolete, will be removed
	if (isPathFromRoot("devices", "device", "modules", "actuator")) {
		logger().warning("tag <actuator> is obsolete, use <control> instead");
		return m_controlExpect
			.find(element.qName) != m_controlExpect.end();
	}

	if (isPathFromRoot("devices", "device", "modules", "control"))
		return m_controlExpect
			.find(element.qName) != m_controlExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "refresh"))
		return m_refreshExpect
			.find(element.qName) != m_refreshExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "*", "constraints"))
		return m_constraintsExpect
			.find(element.qName) != m_constraintsExpect.end();

	if (isPathFromRoot("devices", "device", "modules", "*", "values"))
		return element.qName == "value";

	// obsolete
	if (isPathFromRoot("devices", "device", "modules", "actuator", "rules"))
		return element.qName == "if";

	if (isPathFromRoot("devices", "device", "modules", "control", "rules"))
		return element.qName == "if";

	// obsolete
	if (isPathFromRoot("devices", "device", "modules", "actuator", "rules", "if"))
		return element.qName == "hide-module";

	if (isPathFromRoot("devices", "device", "modules", "control", "rules", "if"))
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


		m_device.setId(DeviceInfoID::parse(id));
		m_device.setName("");
		m_device.setVendor("");
		m_device.clear();
		m_matchers.clear();
	}

	if (isPathFromRoot("devices", "device", "match", "exact")) {
		XMLString name;
		if (!getAndTrimAttribute(attrList, "name", name))
			error("missing attribute name");

		if (name.empty())
			error("name value is empty");

		XMLString vendor;
		if (!getAndTrimAttribute(attrList, "vendor", vendor))
			error("missing attribute vendor");

		if (vendor.empty())
			error("vendor value is empty");

		m_matchers.push_back(new DeviceInfo::MatchExact(name, vendor));
	}

	if (isPathFromRoot("devices", "device", "match", "pattern")) {
		XMLString name;
		if (!getAndTrimAttribute(attrList, "name", name))
			error("missing attribute name");

		if (name.empty())
			error("name value is empty");

		XMLString vendor;
		if (!getAndTrimAttribute(attrList, "vendor", vendor))
			error("missing attribute vendor");

		if (vendor.empty())
			error("vendor value is empty");

		m_matchers.push_back(new DeviceInfo::MatchGlob(name, vendor));
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

		m_module.setId(ModuleInfoID::parse(id));

		const ModuleType &moduleType = ModuleType::parse(type);
		m_module.setType(new TypeInfo(moduleType.type()));

		if (moduleType.customTypeID().isNull()) {
			m_module.setSubtype(nullptr);
		}
		else {
			const string subtype = moduleType.customTypeID().toString();
			EnumInfo enumInfo;
			m_module.setSubtype(new SubtypeInfo(SubtypeInfoID::parse(subtype), enumInfo));
		}

		m_module.setName("");
		m_module.setGroup("");
		m_module.setFromUnknown("");
		m_module.setToUnknown("");
		m_module.setControllable(element.localName == "control" || element.localName == "actuator");
	}

	if (isPathFromRoot("devices", "device", "modules", "*", "unknown")) {
		XMLString from;
		if (getAndTrimAttribute(attrList, "from", from)) {
			if (from.empty())
				error("from value is empty");
		}

		m_module.setFromUnknown(from);

		if (m_module.isControllable()) {
			XMLString to;
			if (getAndTrimAttribute(attrList, "to", to)) {
				if (to.empty())
					error("to value is empty");
			}

			m_module.setToUnknown(to);
		}
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

	if (isPathFromRoot("devices", "device", "vendor"))
		m_device.setVendor(element.content);

	if (isPathFromRoot("devices", "device")) {
		if (m_device.name().empty())
			error("missing name of device " + m_device);

		if (m_result.find(m_device) != m_result.end())
			error("device of id " + m_device + " already exists");

		if (logger().debug())
			logger().debug("inserting device " + m_device, __FILE__, __LINE__);

		DeviceInfo copy(m_device);
		for (const auto matcher : m_matchers)
			copy.addMatch(matcher);

		m_result.insert(copy);
	}
}

void DevicesSAXHandler::preprocessContent(const XMLChar c[],
		int &start, int &length)
{
	trimContent(c, start, length);
}
