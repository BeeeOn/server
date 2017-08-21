#ifndef BEEEON_ROLE_IN_GATEWAY_DAO_H
#define BEEEON_ROLE_IN_GATEWAY_DAO_H

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/Gateway.h"
#include "model/RoleInGateway.h"
#include "model/LegacyRoleInGateway.h"
#include "server/AccessLevel.h"

namespace BeeeOn {

class Identity;
class User;
class VerifiedIdentity;

class RoleInGatewayDao {
public:
	typedef Poco::SharedPtr<RoleInGatewayDao> Ptr;

	virtual ~RoleInGatewayDao();

	virtual void create(RoleInGateway &role) = 0;
	virtual bool update(RoleInGateway &role) = 0;
	virtual bool fetch(RoleInGateway &role) = 0;
	virtual bool fetch(LegacyRoleInGateway &role) = 0;
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

	/**
	 * Test whether the user can see the identity. There must be
	 * an accessible gateway for the user that is also associated
	 * with the given identity.
	 */
	virtual bool canSeeIdentity(
			const Identity &identity,
			const User &user) = 0;
	virtual bool canSeeVerifiedIdentity(
			const VerifiedIdentity &identity,
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

}

#endif
