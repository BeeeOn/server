#ifndef BEEEON_Xml_GATEWAY_DESERIALIZER_H
#define BEEEON_Xml_GATEWAY_DESERIALIZER_H

#include <Poco/DOM/Element.h>

#include "service/Deserializer.h"
#include "model/Gateway.h"

namespace BeeeOn {

class XmlGatewayDeserializer : public Deserializer<Gateway> {
public:
	XmlGatewayDeserializer(const Poco::XML::Element &node);

	void partial(Gateway &gateway) const override;
	void full(Gateway &gateway) const override;

private:
	const Poco::XML::Element &m_node;
};

}

#endif
