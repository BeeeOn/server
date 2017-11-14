#include <cmath>
#include <vector>
#include <Poco/Exception.h>
#include <Poco/Nullable.h>
#include <Poco/NumberParser.h>
#include <Poco/Timespan.h>
#include <Poco/DOM/Element.h>

#include "xmlui/XmlGatewayDeserializer.h"
#include "util/Sanitize.h"

using namespace Poco;
using namespace Poco::XML;
using namespace BeeeOn;

XmlGatewayDeserializer::XmlGatewayDeserializer(const Element &node):
	m_node(node)
{
}

void XmlGatewayDeserializer::setTimeZoneProvider(TimeZoneProvider::Ptr provider)
{
	m_timeZoneProvider = provider;
}

TimeZoneProvider::Ptr XmlGatewayDeserializer::timeZoneProvider() const
{
	return m_timeZoneProvider;
}

static double attributeAsDouble(const Element &e, const std::string &name)
{
	return NumberParser::parseFloat(e.getAttribute(name));
}

static int attributeAsInt(const Element &e, const std::string &name)
{
	return NumberParser::parse(e.getAttribute(name));
}

static Nullable<int> attributeNullable(const std::string &value)
{
	Nullable<int> result;

	if (!value.empty())
		result = NumberParser::parse(value);

	return result;
}

TimeZone XmlGatewayDeserializer::extractTimeZone(const Element &node) const
{
	TimeZoneProvider::Ptr provider = m_timeZoneProvider; // get rid of const

	const Timespan &utcOffset = attributeAsInt(node, "timezone") * Timespan::SECONDS;

	std::vector<TimeZone> zones;
	provider->listTimeZones(zones);

	for (const auto &zone : zones) {
		if (zone.utcOffset() != utcOffset)
			continue;

		return zone;
	}

	throw InvalidArgumentException("fail to detemine timezone");
}

void XmlGatewayDeserializer::partial(Gateway &gateway) const
{
	if (m_node.hasAttribute("name"))
		gateway.setName(Sanitize::common(m_node.getAttribute("name")));

	if (m_node.hasAttribute("longitude"))
		gateway.setLongitude(attributeAsDouble(m_node, "longitude"));

	if (m_node.hasAttribute("latitude"))
		gateway.setLatitude(attributeAsDouble(m_node, "latitude"));

	if (m_node.hasAttribute("altitude"))
		gateway.setAltitude(attributeNullable(m_node.getAttribute("altitude")));

	if (m_node.hasAttribute("timezone"))
		gateway.setTimeZone(extractTimeZone(m_node));
}

void XmlGatewayDeserializer::full(Gateway &gateway) const
{
	if (!m_node.hasAttribute("name"))
		throw InvalidArgumentException("missing name for gateway");

	if (!m_node.hasAttribute("timezone"))
		throw InvalidArgumentException("missing timezone for gateway");

	gateway.setName(Sanitize::common(m_node.getAttribute("name")));

	if (!m_node.hasAttribute("longitude"))
		gateway.setLongitude(NAN);
	else
		gateway.setLongitude(attributeAsDouble(m_node, "longitude"));

	if (!m_node.hasAttribute("latitude"))
		gateway.setLatitude(NAN);
	else
		gateway.setLatitude(attributeAsDouble(m_node, "latitude"));

	if (!m_node.hasAttribute("altitude"))
		gateway.setAltitude(Nullable<int>());
	else
		gateway.setAltitude(attributeNullable(m_node.getAttribute("altitude")));

	gateway.setTimeZone(extractTimeZone(m_node));
}
