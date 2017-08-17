#include <cmath>
#include <Poco/Exception.h>

#include "JSONLocationDeseliazer.h"
#include "util/JsonUtil.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

JSONLocationDeserializer::JSONLocationDeserializer(
        const Object::Ptr object):
        m_object(object)
{
}

void JSONLocationDeserializer::partial(Location &location) const
{
    if (m_object->has("name"))
        location.setName(JsonUtil::extract<string>(m_object, "name"));
}

void JSONLocationDeserializer::full(Location &location) const
{
    if (!m_object->has("name"))
        throw InvalidArgumentException("missing name for location");

    location.setName(JsonUtil::extract<string>(m_object, "name"));
}