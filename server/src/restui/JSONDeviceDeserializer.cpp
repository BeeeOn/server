#include <cmath>
#include <Poco/Exception.h>

#include "restui/JSONDeviceDeserializer.h"
#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

BeeeOn::JSONDeviceDeserializer::JSONDeviceDeserializer(
		const Poco::JSON::Object::Ptr object):
	m_object(object)
{
}

void BeeeOn::JSONDeviceDeserializer::partial(BeeeOn::Device &device) const
{
	if (m_object->has("name"))
		device.setName(Sanitize::common(m_object->getValue<string>("name")));

	if (m_object->has("location_id")) {
		Location location(LocationID::parse(
			m_object->getValue<string>("location_id")));
		device.setLocation(location);
	}

	if (m_object->has("refresh_time"))
		device.setRefresh(m_object->getValue<unsigned int>("refresh_time"));
}

void BeeeOn::JSONDeviceDeserializer::full(BeeeOn::Device &device) const
{
	if (!m_object->has("name"))
		throw new InvalidArgumentException("missing name for device");

	device.setName(Sanitize::common(m_object->getValue<string>("name")));

	if (!m_object->has("location_id"))
		throw InvalidArgumentException("missing location_id for device");

	Location location(LocationID::parse(
			m_object->getValue<string>("location_id")));
	device.setLocation(location);

	if (m_object->has("refresh_time"))
		throw InvalidArgumentException("missing refresh_time for device");

	device.setRefresh(m_object->getValue<unsigned int>("refresh_time"));
}
