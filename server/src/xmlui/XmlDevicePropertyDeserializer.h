#ifndef BEEEON_XML_DEVICE_DESERIALIZER_PROPERTY_H
#define BEEEON_XML_DEVICE_DESERIALIZER_PROPERTY_H

#include <Poco/DOM/Element.h>

#include "service/Deserializer.h"
#include "model/DeviceProperty.h"

namespace BeeeOn {

class XmlDevicePropertyDeserializer : public Deserializer<DeviceProperty> {
public:
	XmlDevicePropertyDeserializer(const Poco::XML::Element &node,
				const CryptoConfig *config);

	void partial(DeviceProperty &property) const override;
	void full(DeviceProperty &property) const override;

protected:
	void deserialize(DeviceProperty &property) const;
	void assureXmlValid() const;
	void assureValid(const DeviceProperty &property) const;
	void applyTo(DeviceProperty &property) const;

private:
	const Poco::XML::Element &m_node;
	const CryptoConfig *m_config;
};

}

#endif


