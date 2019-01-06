#pragma once

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

private:
	const Poco::XML::Element &m_node;
	const CryptoConfig *m_config;
};

}
