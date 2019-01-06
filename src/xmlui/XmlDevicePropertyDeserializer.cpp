#include <Poco/AutoPtr.h>
#include <Poco/Exception.h>
#include <Poco/NumberParser.h>
#include <Poco/DOM/Element.h>
#include <Poco/Net/IPAddress.h>

#include "util/CryptoConfig.h"
#include "xmlui/XmlDevicePropertyDeserializer.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::XML;
using namespace BeeeOn;

XmlDevicePropertyDeserializer::XmlDevicePropertyDeserializer(
		const Element &node, const CryptoConfig *config):
	m_node(node),
	m_config(config)
{
}

void XmlDevicePropertyDeserializer::partial(DeviceProperty &property) const
{
	deserialize(property);
}

void XmlDevicePropertyDeserializer::full(DeviceProperty &property) const
{
	deserialize(property);
}

void XmlDevicePropertyDeserializer::deserialize(DeviceProperty &property) const
{
	assureXmlValid();
	assureValid(property);

	switch (property.key().raw()) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
	case DevicePropertyKey::KEY_PASSWORD:
		property.setParams(m_config->deriveParams());
		break;
	default:
		property.setParams(CryptoParams::createEmpty());
	}

	applyTo(property);
}

void XmlDevicePropertyDeserializer::applyTo(DeviceProperty &property) const
{
	const string &value = m_node.getAttribute("parametervalue");
	property.setFromString(value, *m_config);
}

void XmlDevicePropertyDeserializer::assureXmlValid() const
{
	if (!m_node.hasAttribute("parameterkey"))
		throw InvalidArgumentException("missing parameterkey for device property");

	if (!m_node.hasAttribute("parametervalue"))
		throw InvalidArgumentException("missing parametervalue for device property");
}

void XmlDevicePropertyDeserializer::assureValid(const DeviceProperty &property) const
{
	switch (property.key().raw()) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
	case DevicePropertyKey::KEY_PASSWORD:
		if (m_config == NULL) {
			throw IllegalStateException(
				"missing CryptoConfig to process parameterkey: "
				+ property.key().toString()
			);
		}

		break;
	default:
		throw InvalidArgumentException(
			"unexpected parameterkey value: "
			+ property.key().toString()
		);
	}
}
