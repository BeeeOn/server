#ifndef BEEEON_GATEWAY_H
#define BEEEON_GATEWAY_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"

namespace BeeeOn {

/**
 * Representation of a Gateway application entity.
 */
class Gateway : public Entity {
public:
	typedef Poco::SharedPtr<Gateway> Ptr;
	typedef long ID;

	Gateway(const std::string &data):
		Entity(data)
	{
	}

	Gateway(const Gateway &gateway):
		Entity(gateway)
	{
	}
};

typedef Gateway::Collection GatewayCollection;
typedef Gateway::ID GatewayID;

}

#endif
