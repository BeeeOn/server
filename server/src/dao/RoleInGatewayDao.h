#ifndef BEEEON_ROLE_IN_GATEWAY_DAO_H
#define BEEEON_ROLE_IN_GATEWAY_DAO_H

#include <vector>

#include "di/InjectorTarget.h"
#include "model/Gateway.h"
#include "model/RoleInGateway.h"
#include "model/LegacyRoleInGateway.h"
#include "server/AccessLevel.h"

namespace BeeeOn {

class User;

class RoleInGatewayDao {
public:
	virtual void create(RoleInGateway &role) = 0;
	virtual bool update(RoleInGateway &role) = 0;
	virtual bool fetch(RoleInGateway &role) = 0;
	virtual void fetchBy(std::vector<RoleInGateway> &roles,
			const Gateway &gateway) = 0;
	virtual void fetchBy(std::vector<LegacyRoleInGateway> &roles,
			const Gateway &gateway) = 0;
	virtual bool remove(const RoleInGateway &gateway) = 0;
	virtual bool remove(const Gateway &gateway,
			const User &user) = 0;
	virtual void removeAll(const Gateway &gateway) = 0;

	virtual bool isUser(const RoleInGateway &role, const User &user) = 0;
	virtual bool isRegistered(const Gateway &gateway) = 0;
	virtual bool hasOnlyNonAdminExcept(
			const Gateway &gateway,
			const User &user) = 0;

	virtual AccessLevel fetchAccessLevel(
			const Gateway &gateway,
			const User &user) = 0;

	/**
	 * Fetch all gateways associated with the given user.
	 * I.e. every role with an identity that belongs to
	 * the given user provides access to a gateway. Such
	 * gateways are returned in this call.
	 */
	virtual void fetchAccessibleGateways(
			std::vector<Gateway> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any()) = 0;
};

class NullRoleInGatewayDao : public RoleInGatewayDao {
public:
	NullRoleInGatewayDao();

	void create(RoleInGateway &role) override;
	bool update(RoleInGateway &role) override;
	bool fetch(RoleInGateway &role) override;
	void fetchBy(std::vector<RoleInGateway> &roles,
			const Gateway &gateway) override;
	void fetchBy(std::vector<LegacyRoleInGateway> &roles,
			const Gateway &gateway) override;
	bool remove(const RoleInGateway &role) override;
	bool remove(const Gateway &gateway,
			const User &user)  override;
	void removeAll(const Gateway &gateway) override;

	bool isUser(const RoleInGateway &role, const User &user) override;
	bool isRegistered(const Gateway &gateway) override;
	bool hasOnlyNonAdminExcept(
			const Gateway &gateway,
			const User &user) override;

	AccessLevel fetchAccessLevel(
		const Gateway &gateway,
		const User &user) override;

	void fetchAccessibleGateways(
		std::vector<Gateway> &list,
		const User &user,
		const AccessLevel &atLeast = AccessLevel::any()) override;

	static RoleInGatewayDao &instance();
};

}

#endif
