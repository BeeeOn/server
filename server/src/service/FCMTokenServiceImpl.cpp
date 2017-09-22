#include "di/Injectable.h"
#include "service/FCMTokenServiceImpl.h"

using namespace std;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, FCMTokenServiceImpl)
BEEEON_OBJECT_CASTABLE(FCMTokenService)
BEEEON_OBJECT_REF("fcmTokenDao", &FCMTokenServiceImpl::setFCMTokenDao)
BEEEON_OBJECT_REF("transactionManager", &FCMTokenServiceImpl::setTransactionManager)
BEEEON_OBJECT_REF("accessPolicy", &FCMTokenServiceImpl::setAccessPolicy)
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
	m_dao->insert(input.target());
}

void FCMTokenServiceImpl::doUnregisterToken(Single<FCMToken> &input)
{
	m_accessPolicy->assure(
		FCMTokenAccessPolicy::ACTION_USER_UNREGISTER, input, input.target());
	m_dao->remove(input.target());
}
