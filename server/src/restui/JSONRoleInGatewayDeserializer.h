#ifndef BEEEON_JSON_ROLE_IN_GATEWAY_DESERIALIZER_H
#define BEEEON_JSON_ROLE_IN_GATEWAY_DESERIALIZER_H

#include <istream>
#include <Poco/JSON/Object.h>

#include "model/RoleInGateway.h"
#include "service/Deserializer.h"

namespace BeeeOn {

class JSONRoleInGatewayDeserializer : public Deserializer<RoleInGateway> {
public:
	JSONRoleInGatewayDeserializer(const std::string &input);
	JSONRoleInGatewayDeserializer(std::istream &input);
	JSONRoleInGatewayDeserializer(const Poco::JSON::Object::Ptr data);

	void partial(RoleInGateway &role) const override;
	void full(RoleInGateway &role) const override;

private:
	Poco::JSON::Object::Ptr m_data;
};

}

#endif
