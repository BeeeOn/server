#ifndef BEEEON_ROLE_SERVICE_IMPL_H
#define BEEEON_ROLE_SERVICE_IMPL_H

#include "dao/GatewayDao.h"
#include "policy/RoleAccessPolicy.h"
#include "service/RoleService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class Identity;
class Gateway;
class RoleInGateway;
class LegacyRoleInGateway;
class AccessLevel;

class IdentityDao;
class RoleInGatewayDao;

class NotificationDispatcher;

class RoleServiceImpl : public RoleService, public Transactional {
public:
	RoleServiceImpl();

	void setIdentityDao(IdentityDao *dao);
	void setGatewayDao(GatewayDao::Ptr dao);
	void setRoleInGatewayDao(RoleInGatewayDao *dao);
	void setAccessPolicy(RoleAccessPolicy::Ptr policy);
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
	GatewayDao::Ptr m_gatewayDao;
	RoleInGatewayDao *m_roleInGatewayDao;
	RoleAccessPolicy::Ptr m_accessPolicy;
	NotificationDispatcher *m_notificationDispatcher;
};

}

#endif
