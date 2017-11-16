#include <Poco/Exception.h>
#include <Poco/NumberParser.h>
#include <Poco/DOM/Element.h>

#include "xmlui/XmlDeviceDeserializer.h"
#include "model/Location.h"
#include "util/Sanitize.h"

using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

XmlDeviceDeserializer::XmlDeviceDeserializer(const Element &node):
	m_node(node)
{
}

void XmlDeviceDeserializer::partial(Device &device) const
{
	if (m_node.hasAttribute("name"))
		device.setName(Sanitize::common(m_node.getAttribute("name")));

	if (m_node.hasAttribute("locationid")) {
		device.setLocation(Location(LocationID::parse(
				m_node.getAttribute("locationid"))));
	}
}

void XmlDeviceDeserializer::full(Device &device) const
{
	if (!m_node.hasAttribute("name"))
		throw InvalidArgumentException("missing name for device");

	device.setName(Sanitize::common(m_node.getAttribute("name")));

	if (!m_node.hasAttribute("locationid"))
		throw InvalidArgumentException("missing locationid for device");

	device.setLocation(Location(LocationID::parse(
				m_node.getAttribute("locationid"))));
}
