#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "model/Identity.h"
#include "model/Gateway.h"
#include "model/RoleInGateway.h"
#include "notification/NotificationDispatcher.h"
#include "server/AccessLevel.h"
#include "service/RoleServiceImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, RoleServiceImpl)
BEEEON_OBJECT_CASTABLE(RoleService)
BEEEON_OBJECT_REF("identityDao", &RoleServiceImpl::setIdentityDao)
BEEEON_OBJECT_REF("verifiedIdentityDao", &RoleServiceImpl::setVerifiedIdentityDao)
BEEEON_OBJECT_REF("gatewayDao", &RoleServiceImpl::setGatewayDao)
BEEEON_OBJECT_REF("roleInGatewayDao", &RoleServiceImpl::setRoleInGatewayDao)
BEEEON_OBJECT_REF("accessPolicy", &RoleServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("notificationDispatcher", &RoleServiceImpl::setNotificationDispatcher)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, RoleServiceImpl)

RoleServiceImpl::RoleServiceImpl():
	m_notificationDispatcher(0)
{
}

void RoleServiceImpl::setIdentityDao(IdentityDao::Ptr dao)
{
	m_identityDao = dao;
}

void RoleServiceImpl::setVerifiedIdentityDao(VerifiedIdentityDao::Ptr dao)
{
	m_verifiedIdentityDao = dao;
}

void RoleServiceImpl::setGatewayDao(GatewayDao::Ptr dao)
{
	m_gatewayDao = dao;
}

void RoleServiceImpl::setRoleInGatewayDao(RoleInGatewayDao::Ptr dao)
{
	m_roleInGatewayDao = dao;
}

void RoleServiceImpl::setAccessPolicy(RoleAccessPolicy::Ptr policy)
{
	m_accessPolicy = policy;
}

void RoleServiceImpl::setNotificationDispatcher(
		NotificationDispatcher *dispatcher)
{
	m_notificationDispatcher = dispatcher;
}

void RoleServiceImpl::doInviteIdentity(
		Relation<RoleInGateway, Gateway> &input,
		const Identity &identity,
		const AccessLevel &as)
{
	m_accessPolicy->assure(RoleAccessPolicy::ACTION_USER_INVITE, input, input.base());

	Identity tmp(identity);

	if (!m_identityDao->fetchBy(tmp, identity.email()))
		m_identityDao->create(tmp);

	RoleInGateway &role = input.target();
	role.setGateway(input.base());
	role.setIdentity(tmp);
	role.setLevel(as);

	// Create the role or fail if already exists.
	m_roleInGatewayDao->create(role);

	// notify about the invitation
	m_notificationDispatcher->notifyInvited(
			tmp, input.base(), input.user());
}

bool RoleServiceImpl::doFetch(Relation<LegacyRoleInGateway, Gateway> &input)
{
	m_accessPolicy->assure(RoleAccessPolicy::ACTION_USER_GET, input, input.target());
	return m_roleInGatewayDao->fetch(input.target());
}

bool RoleServiceImpl::doFetch(Relation<RoleInGateway, Gateway> &input, const VerifiedIdentity &identity)
{
	m_accessPolicy->assure(RoleAccessPolicy::ACTION_USER_GET, input, input.base());

	VerifiedIdentity tmp(identity);
	if (!m_verifiedIdentityDao->fetch(tmp))
		throw NotFoundException("no such verified identity " + identity);

	vector<RoleInGateway> roles;
	m_roleInGatewayDao->fetchBy(roles, input.base());

	for (const auto &role : roles) {
		if (role.identity().id() == tmp.identity().id()) {
			input.target() = role;
			return true;
		}
	}

	return false;
}

void RoleServiceImpl::doList(Relation<vector<RoleInGateway>, Gateway> &input)
{
	m_accessPolicy->assure(RoleAccessPolicy::ACTION_USER_GET, input, input.base());
	m_roleInGatewayDao->fetchBy(input.target(), input.base());
}

void RoleServiceImpl::doList(Relation<vector<LegacyRoleInGateway>, Gateway> &input)
{
	m_accessPolicy->assure(RoleAccessPolicy::ACTION_USER_GET, input, input.base());
	m_roleInGatewayDao->fetchBy(input.target(), input.base());
}

void RoleServiceImpl::doRemove(Single<RoleInGateway> &input)
{
	m_accessPolicy->assure(RoleAccessPolicy::ACTION_USER_REMOVE, input, input.target());

	if (m_roleInGatewayDao->isUser(input.target(), input.user()))
		throw IllegalStateException("cannot remove self");

	if (!m_roleInGatewayDao->remove(input.target())) {
		logger().warning("failed to remove role " + input.target(),
				__FILE__, __LINE__);
	}
}

void RoleServiceImpl::doUpdate(SingleWithData<RoleInGateway> &input)
{
	m_accessPolicy->assure(RoleAccessPolicy::ACTION_USER_UPDATE, input, input.target());

	RoleInGateway &role = input.target();

	if (!m_roleInGatewayDao->fetch(role))
		throw NotFoundException("no such role " + role);

	input.data().full(role);

	if (!m_roleInGatewayDao->update(role))
		throw IllegalStateException("failed to update role " + role);
}
