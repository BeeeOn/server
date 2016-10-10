#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>

#include "service/JSONLocationDeserializer.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

JSONLocationDeserializer::JSONLocationDeserializer(const string &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONLocationDeserializer::JSONLocationDeserializer(istream &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONLocationDeserializer::JSONLocationDeserializer(const Object::Ptr data):
	m_data(data)
{
}

void JSONLocationDeserializer::partial(Location &location) const
{
	if (m_data->has("name"))
		location.setName(m_data->getValue<string>("name"));
}

void JSONLocationDeserializer::full(Location &location) const
{
	if (!m_data->has("name"))
		throw InvalidArgumentException("missing name for location");

	location.setName(m_data->getValue<string>("name"));
}
