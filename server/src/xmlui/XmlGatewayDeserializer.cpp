#include <cmath>
#include <Poco/Exception.h>
#include <Poco/NumberParser.h>
#include <Poco/DOM/Element.h>

#include "xmlui/XmlGatewayDeserializer.h"

using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

XmlGatewayDeserializer::XmlGatewayDeserializer(const Element &node):
	m_node(node)
{
}

static double attributeAsDouble(const Element &e, const std::string &name)
{
	return NumberParser::parseFloat(e.getAttribute(name));
}

void XmlGatewayDeserializer::partial(Gateway &gateway) const
{
	if (m_node.hasAttribute("name"))
		gateway.setName(m_node.getAttribute("name"));

	if (m_node.hasAttribute("longitude"))
		gateway.setLongitude(attributeAsDouble(m_node, "longitude"));

	if (m_node.hasAttribute("latitude"))
		gateway.setLatitude(attributeAsDouble(m_node, "latitude"));

	if (m_node.hasAttribute("altitude"))
		gateway.setAltitude(attributeAsDouble(m_node, "altitude"));
}

void XmlGatewayDeserializer::full(Gateway &gateway) const
{
	if (!m_node.hasAttribute("name"))
		throw InvalidArgumentException("missing name for gateway");

	gateway.setName(m_node.getAttribute("name"));

	if (!m_node.hasAttribute("longitude"))
		gateway.setLongitude(NAN);
	else
		gateway.setLongitude(attributeAsDouble(m_node, "longitude"));

	if (!m_node.hasAttribute("latitude"))
		gateway.setLatitude(NAN);
	else
		gateway.setLatitude(attributeAsDouble(m_node, "latitude"));

	if (!m_node.hasAttribute("altitude"))
		gateway.setAltitude(NAN);
	else
		gateway.setAltitude(attributeAsDouble(m_node, "altitude"));
}
