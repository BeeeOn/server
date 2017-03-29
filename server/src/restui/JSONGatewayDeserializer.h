#ifndef BEEEON_JSON_GATEWAY_DESERIALIZER_H
#define BEEEON_JSON_GATEWAY_DESERIALIZER_H

#include <Poco/JSON/Object.h>

#include "service/Deserializer.h"
#include "model/Gateway.h"

namespace BeeeOn {

class JSONGatewayDeserializer : public Deserializer<Gateway> {
public:
	JSONGatewayDeserializer(const Poco::JSON::Object::Ptr object);

	void partial(Gateway &gateway) const override;
	void full(Gateway &gateway) const override;

private:
	const Poco::JSON::Object::Ptr m_object;
};

}

#endif
