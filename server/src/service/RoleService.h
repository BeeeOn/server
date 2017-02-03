#ifndef BEEEON_ROLE_SERVICE_H
#define BEEEON_ROLE_SERVICE_H

#include "dao/Transactional.h"
#include "service/Single.h"
#include "service/Relation.h"

namespace BeeeOn {

class Identity;
class Gateway;
class RoleInGateway;
class LegacyRoleInGateway;
class AccessLevel;

class IdentityDao;
class GatewayDao;
class RoleInGatewayDao;

class RoleAccessPolicy;
class NotificationDispatcher;

class RoleService : public Transactional {
public:
	RoleService();

	void setIdentityDao(IdentityDao *dao);
	void setGatewayDao(GatewayDao *dao);
	void setRoleInGatewayDao(RoleInGatewayDao *dao);
	void setAccessPolicy(RoleAccessPolicy *policy);
	void setNotificationDispatcher(NotificationDispatcher *service);

	void inviteIdentity(
			Relation<Identity, Gateway> &input,
			const AccessLevel &as)
	{
		BEEEON_TRANSACTION(doInviteIdentity(input, as));
	}

	void list(Relation<std::vector<RoleInGateway>, Gateway> &input)
	{
		BEEEON_TRANSACTION(doList(input));
	}

	void list(Relation<std::vector<LegacyRoleInGateway>, Gateway> &input)
	{
		BEEEON_TRANSACTION(doList(input));
	}

	void remove(Single<RoleInGateway> &input)
	{
		BEEEON_TRANSACTION(doRemove(input));
	}

	void update(SingleWithData<RoleInGateway> &input)
	{
		BEEEON_TRANSACTION(doUpdate(input));
	}

protected:
	void doInviteIdentity(
			Relation<Identity, Gateway> &input,
			const AccessLevel &as);
	void doList(Relation<std::vector<RoleInGateway>, Gateway> &input);
	void doList(Relation<std::vector<LegacyRoleInGateway>, Gateway> &input);
	void doRemove(Single<RoleInGateway> &input);
	void doUpdate(SingleWithData<RoleInGateway> &input);

private:
	IdentityDao *m_identityDao;
	GatewayDao *m_gatewayDao;
	RoleInGatewayDao *m_roleInGatewayDao;
	RoleAccessPolicy *m_accessPolicy;
	NotificationDispatcher *m_notificationDispatcher;
};

}

#endif
