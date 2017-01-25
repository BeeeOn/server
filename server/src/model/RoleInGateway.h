#ifndef BEEEON_ROLE_IN_GATEWAY_H
#define BEEEON_ROLE_IN_GATEWAY_H

#include <Poco/SharedPtr.h>

#include "model/Entity.h"
#include "model/GlobalID.h"
#include "model/Gateway.h"
#include "model/Identity.h"
#include "server/AccessLevel.h"

namespace Poco {

class DateTime;

}

namespace BeeeOn {

class RoleInGateway : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<RoleInGateway> Ptr;

	RoleInGateway();
	RoleInGateway(const ID &id);

	void setGateway(const Gateway &gateway);
	const Gateway &gateway() const;

	void setIdentity(const Identity &identity);
	const Identity &identity() const;

	void setLevel(const AccessLevel &level);
	const AccessLevel &level() const;

	void setCreated(const Poco::DateTime &created);
	const Poco::DateTime &created() const;

private:
	Gateway m_gateway;
	Identity m_identity;
	AccessLevel m_level;
	Poco::DateTime m_created;
};

typedef RoleInGateway::ID RoleInGatewayID;

}

#endif
