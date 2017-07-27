#include <Poco/Exception.h>

#include "dao/IdentityDao.h"
#include "dao/GatewayDao.h"
#include "dao/RoleInGatewayDao.h"
#include "di/Injectable.h"
#include "model/Identity.h"
#include "model/Gateway.h"
#include "model/RoleInGateway.h"
#include "notification/NotificationDispatcher.h"
#include "policy/RoleAccessPolicy.h"
#include "server/AccessLevel.h"
#include "service/RoleService.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, RoleService)
BEEEON_OBJECT_REF("identityDao", &RoleService::setIdentityDao)
BEEEON_OBJECT_REF("gatewayDao", &RoleService::setGatewayDao)
BEEEON_OBJECT_REF("roleInGatewayDao", &RoleService::setRoleInGatewayDao)
BEEEON_OBJECT_REF("accessPolicy", &RoleService::setAccessPolicy)
BEEEON_OBJECT_REF("notificationDispatcher", &RoleService::setNotificationDispatcher)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, RoleService)

RoleService::RoleService():
	m_identityDao(&NullIdentityDao::instance()),
	m_roleInGatewayDao(&NullRoleInGatewayDao::instance()),
	m_notificationDispatcher(0)
{
}

void RoleService::setIdentityDao(IdentityDao *dao)
{
	m_identityDao = dao == NULL?
		&NullIdentityDao::instance() : dao;
}

void RoleService::setGatewayDao(GatewayDao *dao)
{
	m_gatewayDao = dao == NULL?
		&NullGatewayDao::instance() : dao;
}

void RoleService::setRoleInGatewayDao(RoleInGatewayDao *dao)
{
	m_roleInGatewayDao = dao == NULL?
		&NullRoleInGatewayDao::instance() : dao;
}

void RoleService::setAccessPolicy(RoleAccessPolicy *policy)
{
	m_accessPolicy = policy == NULL?
		&NullRoleAccessPolicy::instance() : policy;
}

void RoleService::setNotificationDispatcher(
		NotificationDispatcher *dispatcher)
{
	m_notificationDispatcher = dispatcher;
}

void RoleService::doInviteIdentity(
		Relation<Identity, Gateway> &input,
		const AccessLevel &as)
{
	m_accessPolicy->assureInvite(input, input.base(), as);

	const string email(input.target().email());

	if (!m_identityDao->fetchBy(input.target(), email))
		m_identityDao->create(input.target());

	RoleInGateway role;
	role.setGateway(input.base());
	role.setIdentity(input.target());
	role.setLevel(as);

	// Create the role or fail if already exists.
	m_roleInGatewayDao->create(role);

	// notify about the invitation
	m_notificationDispatcher->notifyInvited(
			input.target(),
			input.base(),
			input.user());
}

void RoleService::doList(Relation<vector<RoleInGateway>, Gateway> &input)
{
	m_accessPolicy->assureList(input, input.base());
	m_roleInGatewayDao->fetchBy(input.target(), input.base());
}

void RoleService::doList(Relation<vector<LegacyRoleInGateway>, Gateway> &input)
{
	m_accessPolicy->assureList(input, input.base());
	m_roleInGatewayDao->fetchBy(input.target(), input.base());
}

void RoleService::doRemove(Single<RoleInGateway> &input)
{
	m_accessPolicy->assureRemove(input, input.target());

	if (m_roleInGatewayDao->isUser(input.target(), input.user()))
		throw IllegalStateException("cannot remove self");

	if (!m_roleInGatewayDao->remove(input.target())) {
		logger().warning("failed to remove role " + input.target(),
				__FILE__, __LINE__);
	}
}

void RoleService::doUpdate(SingleWithData<RoleInGateway> &input)
{
	m_accessPolicy->assureUpdate(input, input.target());

	RoleInGateway &role = input.target();

	if (!m_roleInGatewayDao->fetch(role))
		throw NotFoundException("no such role " + role);

	input.data().full(role);

	if (!m_roleInGatewayDao->update(role))
		throw IllegalStateException("failed to update role " + role);
}
