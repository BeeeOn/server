#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>

#include "restui/JSONGatewayDeserializer.h"

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

void JSONGatewayDeserializer::partial(Gateway &gateway) const
{
	if (m_data->has("name"))
		gateway.setName(m_data->getValue<string>("name"));

	if (m_data->has("altitude"))
		gateway.setAltitude(m_data->getValue<int>("altitude"));

	if (m_data->has("latitude"))
		gateway.setLatitude(m_data->getValue<double>("latitude"));

	if (m_data->has("longitude"))
		gateway.setLongitude(m_data->getValue<double>("longitude"));
}

void JSONGatewayDeserializer::full(Gateway &gateway) const
{
	if (!m_data->has("name"))
		throw InvalidArgumentException("missing name for gateway");

	gateway.setName(m_data->getValue<string>("name"));

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
}
