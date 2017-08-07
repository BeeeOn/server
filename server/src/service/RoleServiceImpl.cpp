#include <Poco/Exception.h>

#include "dao/IdentityDao.h"
#include "dao/GatewayDao.h"
#include "dao/RoleInGatewayDao.h"
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
BEEEON_OBJECT_REF("gatewayDao", &RoleServiceImpl::setGatewayDao)
BEEEON_OBJECT_REF("roleInGatewayDao", &RoleServiceImpl::setRoleInGatewayDao)
BEEEON_OBJECT_REF("accessPolicy", &RoleServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("notificationDispatcher", &RoleServiceImpl::setNotificationDispatcher)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, RoleServiceImpl)

RoleServiceImpl::RoleServiceImpl():
	m_identityDao(&NullIdentityDao::instance()),
	m_roleInGatewayDao(&NullRoleInGatewayDao::instance()),
	m_notificationDispatcher(0)
{
}

void RoleServiceImpl::setIdentityDao(IdentityDao *dao)
{
	m_identityDao = dao == NULL?
		&NullIdentityDao::instance() : dao;
}

void RoleServiceImpl::setGatewayDao(GatewayDao *dao)
{
	m_gatewayDao = dao == NULL?
		&NullGatewayDao::instance() : dao;
}

void RoleServiceImpl::setRoleInGatewayDao(RoleInGatewayDao *dao)
{
	m_roleInGatewayDao = dao == NULL?
		&NullRoleInGatewayDao::instance() : dao;
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

void RoleServiceImpl::doList(Relation<vector<RoleInGateway>, Gateway> &input)
{
	m_accessPolicy->assureList(input, input.base());
	m_roleInGatewayDao->fetchBy(input.target(), input.base());
}

void RoleServiceImpl::doList(Relation<vector<LegacyRoleInGateway>, Gateway> &input)
{
	m_accessPolicy->assureList(input, input.base());
	m_roleInGatewayDao->fetchBy(input.target(), input.base());
}

void RoleServiceImpl::doRemove(Single<RoleInGateway> &input)
{
	m_accessPolicy->assureRemove(input, input.target());

	if (m_roleInGatewayDao->isUser(input.target(), input.user()))
		throw IllegalStateException("cannot remove self");

	if (!m_roleInGatewayDao->remove(input.target())) {
		logger().warning("failed to remove role " + input.target(),
				__FILE__, __LINE__);
	}
}

void RoleServiceImpl::doUpdate(SingleWithData<RoleInGateway> &input)
{
	m_accessPolicy->assureUpdate(input, input.target());

	RoleInGateway &role = input.target();

	if (!m_roleInGatewayDao->fetch(role))
		throw NotFoundException("no such role " + role);

	input.data().full(role);

	if (!m_roleInGatewayDao->update(role))
		throw IllegalStateException("failed to update role " + role);
}
