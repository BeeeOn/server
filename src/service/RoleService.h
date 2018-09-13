#pragma once

#include <vector>

#include <Poco/SharedPtr.h>

#include "service/Single.h"
#include "service/Relation.h"

namespace BeeeOn {

class Identity;
class Gateway;
class RoleInGateway;
class LegacyRoleInGateway;
class AccessLevel;
class VerifiedIdentity;

class RoleService {
public:
	typedef Poco::SharedPtr<RoleService> Ptr;

	virtual ~RoleService();

	virtual void inviteIdentity(
			Relation<RoleInGateway, Gateway> &input,
			const Identity &identity,
			const AccessLevel &as) = 0;
	virtual bool fetch(Relation<LegacyRoleInGateway, Gateway> &input) = 0;
	virtual bool fetch(Relation<RoleInGateway, Gateway> &input, const VerifiedIdentity &identity) = 0;
	virtual void list(Relation<std::vector<RoleInGateway>, Gateway> &input) = 0;
	virtual void list(Relation<std::vector<LegacyRoleInGateway>, Gateway> &input) = 0;
	virtual void remove(Single<RoleInGateway> &input) = 0;
	virtual void update(SingleWithData<RoleInGateway> &input) = 0;
};

}
