#ifndef BEEEON_XML_ROLE_IN_GATEWAY_DESERIALIZER_H
#define BEEEON_XML_ROLE_IN_GATEWAY_DESERIALIZER_H

#include "service/Deserializer.h"
#include "model/RoleInGateway.h"

namespace Poco {
namespace XML {

class Element;

}
}

namespace BeeeOn {

class XmlRoleInGatewayDeserializer : public Deserializer<RoleInGateway> {
public:
	XmlRoleInGatewayDeserializer(const Poco::XML::Element &node);

	void partial(RoleInGateway &role) const override;
	void full(RoleInGateway &role) const override;

private:
	const Poco::XML::Element &m_node;
};

}

#endif
