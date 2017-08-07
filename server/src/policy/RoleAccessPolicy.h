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

	virtual ~RoleAccessPolicy();

	virtual void assureInvite(
		const PolicyContext &context,
		const Gateway &gateway,
		const AccessLevel &as) = 0;

	virtual void assureList(
		const PolicyContext &context,
		const Gateway &gateway) = 0;

	virtual void assureRemove(
		const PolicyContext &context,
		const RoleInGateway &role) = 0;

	virtual void assureUpdate(
		const PolicyContext &context,
		const RoleInGateway &role) = 0;
};

}

#endif
