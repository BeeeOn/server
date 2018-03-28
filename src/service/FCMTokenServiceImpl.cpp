#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "service/FCMTokenServiceImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, FCMTokenServiceImpl)
BEEEON_OBJECT_CASTABLE(FCMTokenService)
BEEEON_OBJECT_PROPERTY("fcmTokenDao", &FCMTokenServiceImpl::setFCMTokenDao)
BEEEON_OBJECT_PROPERTY("transactionManager", &FCMTokenServiceImpl::setTransactionManager)
BEEEON_OBJECT_PROPERTY("accessPolicy", &FCMTokenServiceImpl::setAccessPolicy)
BEEEON_OBJECT_END(BeeeOn, FCMTokenServiceImpl)

FCMTokenServiceImpl::FCMTokenServiceImpl()
{
}

void FCMTokenServiceImpl::setFCMTokenDao(FCMTokenDao::Ptr dao)
{
	m_dao = dao;
}

void FCMTokenServiceImpl::setAccessPolicy(FCMTokenAccessPolicy::Ptr accessPolicy)
{
	m_accessPolicy = accessPolicy;
}

void FCMTokenServiceImpl::doRegisterToken(Single<FCMToken> &input)
{
	m_accessPolicy->assure(
		FCMTokenAccessPolicy::ACTION_USER_REGISTER, input, input.target());

	FCMToken token(input.target());

	if (!m_dao->fetch(token)) {
		m_dao->insert(input.target());
		return;
	}

	if (token.user().id() == input.target().user().id())
		return;

	throw InvalidArgumentException("same token for different users");
}

void FCMTokenServiceImpl::doUnregisterToken(Single<FCMToken> &input)
{
	m_accessPolicy->assure(
		FCMTokenAccessPolicy::ACTION_USER_UNREGISTER, input, input.target());
	m_dao->remove(input.target());
}
