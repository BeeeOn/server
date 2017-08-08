#ifndef BEEEON_ROLE_ACCESS_POLICY_H
#define BEEEON_ROLE_ACCESS_POLICY_H

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class Identity;
class Gateway;
class RoleInGateway;
class PolicyContext;
class AccessLevel;

class RoleAccessPolicy {
public:
	typedef Poco::SharedPtr<RoleAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_INVITE,
		ACTION_USER_GET,
		ACTION_USER_UPDATE,
		ACTION_USER_REMOVE,
	};

	virtual ~RoleAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Gateway &gateway) = 0;

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const RoleInGateway &role) = 0;
};

}

#endif
