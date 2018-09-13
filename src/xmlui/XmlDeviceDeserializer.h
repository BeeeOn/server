#pragma once

#include <Poco/DOM/Element.h>

#include "service/Deserializer.h"
#include "model/Device.h"

namespace BeeeOn {

class XmlDeviceDeserializer : public Deserializer<Device> {
public:
	XmlDeviceDeserializer(const Poco::XML::Element &node);

	void partial(Device &device) const override;
	void full(Device &device) const override;

private:
	const Poco::XML::Element &m_node;
};

}
