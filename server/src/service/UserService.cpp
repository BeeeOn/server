#include "di/Injectable.h"
#include "service/UserService.h"

using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, UserService)
BEEEON_OBJECT_REF("userDao", &UserService::setUserDao)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, UserService)

UserService::UserService()
{
}

void UserService::setUserDao(UserDao *dao)
{
	m_dao = dao;
}

void UserService::doCreate(User &u)
{
	m_dao->create(u);
}

bool UserService::doFetch(User &u)
{
	return m_dao->fetch(u);
}
