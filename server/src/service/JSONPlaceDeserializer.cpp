#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>

#include "service/JSONPlaceDeserializer.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

JSONPlaceDeserializer::JSONPlaceDeserializer(const string &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONPlaceDeserializer::JSONPlaceDeserializer(istream &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONPlaceDeserializer::JSONPlaceDeserializer(const Object::Ptr data):
	m_data(data)
{
}

void JSONPlaceDeserializer::partial(Place &place) const
{
	if (m_data->has("name"))
		place.setName(m_data->getValue<string>("name"));
}

void JSONPlaceDeserializer::full(Place &place) const
{
	if (!m_data->has("name"))
		throw InvalidArgumentException("missing name for place");

	place.setName(m_data->getValue<string>("name"));
}
