#ifndef BEEEON_ROLE_ACCESS_POLICY_H
#define BEEEON_ROLE_ACCESS_POLICY_H

namespace BeeeOn {

class Identity;
class Gateway;
class RoleInGateway;
class PolicyContext;
class AccessLevel;

class RoleAccessPolicy {
public:
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

class NullRoleAccessPolicy : public RoleAccessPolicy {
public:
	NullRoleAccessPolicy();

	void assureInvite(
		const PolicyContext &context,
		const Gateway &gateway,
		const AccessLevel &as) override;

	void assureList(
		const PolicyContext &context,
		const Gateway &gateway) override;

	void assureRemove(
		const PolicyContext &context,
		const RoleInGateway &role) override;

	void assureUpdate(
		const PolicyContext &context,
		const RoleInGateway &role) override;

	static RoleAccessPolicy &instance();
};

}

#endif
