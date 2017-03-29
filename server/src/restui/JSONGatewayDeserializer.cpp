#include <cmath>
#include <Poco/Exception.h>

#include "restui/JSONGatewayDeserializer.h"
#include "util/JsonUtil.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

JSONGatewayDeserializer::JSONGatewayDeserializer(
		const Object::Ptr object):
	m_object(object)
{
}

void JSONGatewayDeserializer::partial(Gateway &gateway) const
{
	if (m_object->has("name"))
		gateway.setName(JsonUtil::extract<string>(m_object, "name"));

	if (m_object->has("longitude"))
		gateway.setLongitude(JsonUtil::extract<double>(m_object, "longitude"));

	if (m_object->has("latitude"))
		gateway.setLatitude(JsonUtil::extract<double>(m_object, "latitude"));

	if (m_object->has("altitude"))
		gateway.setAltitude(JsonUtil::extract<double>(m_object, "altitude"));
}

void JSONGatewayDeserializer::full(Gateway &gateway) const
{
	if (!m_object->has("name"))
		throw InvalidArgumentException("missing name for gateway");

	gateway.setName(JsonUtil::extract<string>(m_object, "name"));

	if (!m_object->has("longitude"))
		gateway.setLongitude(NAN);
	else
		gateway.setLongitude(JsonUtil::extract<double>(m_object, "longitude"));

	if (!m_object->has("latitude"))
		gateway.setLatitude(NAN);
	else
		gateway.setLatitude(JsonUtil::extract<double>(m_object, "latitude"));

	if (!m_object->has("altitude"))
		gateway.setAltitude(NAN);
	else
		gateway.setAltitude(JsonUtil::extract<double>(m_object, "altitude"));
}
