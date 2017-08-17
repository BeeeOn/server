#ifndef BEEEON_ROLE_SERVICE_IMPL_H
#define BEEEON_ROLE_SERVICE_IMPL_H

#include "dao/GatewayDao.h"
#include "dao/IdentityDao.h"
#include "dao/RoleInGatewayDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "policy/RoleAccessPolicy.h"
#include "service/RoleService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class Identity;
class Gateway;
class RoleInGateway;
class LegacyRoleInGateway;
class AccessLevel;

class NotificationDispatcher;

class RoleServiceImpl : public RoleService, public Transactional {
public:
	RoleServiceImpl();

	void setIdentityDao(IdentityDao::Ptr dao);
	void setVerifiedIdentityDao(VerifiedIdentityDao::Ptr dao);
	void setGatewayDao(GatewayDao::Ptr dao);
	void setRoleInGatewayDao(RoleInGatewayDao::Ptr dao);
	void setAccessPolicy(RoleAccessPolicy::Ptr policy);
	void setNotificationDispatcher(NotificationDispatcher *service);

	void inviteIdentity(
			Relation<Identity, Gateway> &input,
			const AccessLevel &as)
	{
		BEEEON_TRANSACTION(doInviteIdentity(input, as));
	}

	bool fetch(Relation<LegacyRoleInGateway, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	bool fetch(Relation<RoleInGateway, Gateway> &input, const VerifiedIdentity &identity)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input, identity));
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
	bool doFetch(Relation<LegacyRoleInGateway, Gateway> &input);
	bool doFetch(Relation<RoleInGateway, Gateway> &input, const VerifiedIdentity &identity);
	void doList(Relation<std::vector<RoleInGateway>, Gateway> &input);
	void doList(Relation<std::vector<LegacyRoleInGateway>, Gateway> &input);
	void doRemove(Single<RoleInGateway> &input);
	void doUpdate(SingleWithData<RoleInGateway> &input);

private:
	IdentityDao::Ptr m_identityDao;
	VerifiedIdentityDao::Ptr m_verifiedIdentityDao;
	GatewayDao::Ptr m_gatewayDao;
	RoleInGatewayDao::Ptr m_roleInGatewayDao;
	RoleAccessPolicy::Ptr m_accessPolicy;
	NotificationDispatcher *m_notificationDispatcher;
};

}

#endif
