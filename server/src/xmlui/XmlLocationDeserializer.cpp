#include <cmath>
#include <Poco/Exception.h>
#include <Poco/NumberParser.h>
#include <Poco/DOM/Element.h>

#include "xmlui/XmlLocationDeserializer.h"
#include "util/Sanitize.h"

using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

XmlLocationDeserializer::XmlLocationDeserializer(const Element &node):
	m_node(node)
{
}

void XmlLocationDeserializer::partial(Location &location) const
{
	if (m_node.hasAttribute("name"))
		location.setName(Sanitize::common(m_node.getAttribute("name")));
}

void XmlLocationDeserializer::full(Location &location) const
{
	if (!m_node.hasAttribute("name"))
		throw InvalidArgumentException("missing name for location");

	location.setName(Sanitize::common(m_node.getAttribute("name")));
}
