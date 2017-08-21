#include <Poco/Exception.h>
#include <Poco/JSON/Parser.h>

#include "restui/JSONRoleInGatewayDeserializer.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

JSONRoleInGatewayDeserializer::JSONRoleInGatewayDeserializer(const string &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONRoleInGatewayDeserializer::JSONRoleInGatewayDeserializer(istream &input)
{
	Parser parser;
	m_data = parser.parse(input).extract<Object::Ptr>();
}

JSONRoleInGatewayDeserializer::JSONRoleInGatewayDeserializer(const Object::Ptr data):
	m_data(data)
{
}

void JSONRoleInGatewayDeserializer::partial(RoleInGateway &role) const
{
	if (m_data->has("access_level"))
		role.setLevel(AccessLevel::parse(m_data->getValue<string>("access_level")));
}

void JSONRoleInGatewayDeserializer::full(RoleInGateway &role) const
{
	if (!m_data->has("access_level"))
		throw InvalidArgumentException("missing access_level for role");

	role.setLevel(AccessLevel::parse(m_data->getValue<string>("access_level")));
}
