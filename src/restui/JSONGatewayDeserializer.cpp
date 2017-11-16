#include <Poco/Exception.h>
#include <Poco/Nullable.h>
#include <Poco/JSON/Parser.h>

#include "restui/JSONGatewayDeserializer.h"
#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

JSONGatewayDeserializer::JSONGatewayDeserializer(const string &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONGatewayDeserializer::JSONGatewayDeserializer(istream &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONGatewayDeserializer::JSONGatewayDeserializer(const Object::Ptr data):
	m_data(data)
{
}

void JSONGatewayDeserializer::setTimeZoneProvider(TimeZoneProvider::Ptr provider)
{
	m_timeZoneProvider = provider;
}

TimeZoneProvider::Ptr JSONGatewayDeserializer::timeZoneProvider() const
{
	return m_timeZoneProvider;
}

TimeZone JSONGatewayDeserializer::extractTimeZone(const Object::Ptr data) const
{
	TimeZoneProvider::Ptr provider = m_timeZoneProvider; // get rid of const

	const string &id = Sanitize::common(m_data->getValue<string>("timezone_id"));
	Nullable<TimeZone> timeZone = provider->findById(id);

	if (timeZone.isNull())
		throw InvalidArgumentException("unrecognized timezone_id given");

	return timeZone.value();
}

void JSONGatewayDeserializer::partial(Gateway &gateway) const
{
	if (m_data->has("name"))
		gateway.setName(Sanitize::common(m_data->getValue<string>("name")));

	if (m_data->has("altitude"))
		gateway.setAltitude(m_data->getValue<int>("altitude"));

	if (m_data->has("latitude"))
		gateway.setLatitude(m_data->getValue<double>("latitude"));

	if (m_data->has("longitude"))
		gateway.setLongitude(m_data->getValue<double>("longitude"));

	if (m_data->has("timezone_id")) {
		const TimeZone &timeZone = extractTimeZone(m_data);
		gateway.setTimeZone(timeZone);
	}
}

void JSONGatewayDeserializer::full(Gateway &gateway) const
{
	if (!m_data->has("name"))
		throw InvalidArgumentException("missing name for gateway");

	if (!m_data->has("timezone_id"))
		throw InvalidArgumentException("missing timezone_id for gateway");

	gateway.setName(Sanitize::common(m_data->getValue<string>("name")));

	if (!m_data->has("altitude"))
		gateway.setAltitude(0);
	else
		gateway.setAltitude(m_data->getValue<int>("altitude"));

	if (!m_data->has("latitude"))
		gateway.setLatitude(NAN);
	else
		gateway.setLatitude(m_data->getValue<double>("latitude"));

	if (!m_data->has("longitude"))
		gateway.setLongitude(NAN);
	else
		gateway.setLongitude(m_data->getValue<double>("longitude"));

	const TimeZone &timeZone = extractTimeZone(m_data);
	gateway.setTimeZone(timeZone);
}
