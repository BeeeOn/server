#include <Poco/Exception.h>
#include <Poco/Crypto/Cipher.h>
#include <Poco/Crypto/CipherFactory.h>

#include "restui/JSONDevicePropertyDeserializer.h"
#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace Poco::Crypto;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace BeeeOn;

JSONDevicePropertyDeserializer::JSONDevicePropertyDeserializer(
		const Poco::JSON::Object::Ptr object,
		const SharedPtr<CryptoConfig> config):
	m_object(object),
	m_cryptoConfig(config)
{
}

void JSONDevicePropertyDeserializer::assureWritable(const DeviceProperty &property) const
{
	if (!property.key().isUserWritable()) {
		throw InvalidAccessException(
			"property " + property.key().toString()
			+ " is not writable");
	}
}

void JSONDevicePropertyDeserializer::deserialize(DeviceProperty &property) const
{
	assureWritable(property);

	const auto value = m_object->getValue<string>("value");

	property.setParams(property.key().deriveParams(*m_cryptoConfig));
	property.setFromString(value, *m_cryptoConfig);
}

void JSONDevicePropertyDeserializer::partial(DeviceProperty &property) const
{
	deserialize(property);
}

void JSONDevicePropertyDeserializer::full(DeviceProperty &property) const
{
	deserialize(property);
}
