#include "di/Injectable.h"
#include "service/UserServiceImpl.h"

using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, UserServiceImpl)
BEEEON_OBJECT_CASTABLE(UserService)
BEEEON_OBJECT_PROPERTY("userDao", &UserServiceImpl::setUserDao)
BEEEON_OBJECT_PROPERTY("transactionManager", &UserServiceImpl::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, UserServiceImpl)

UserServiceImpl::UserServiceImpl()
{
}

void UserServiceImpl::setUserDao(UserDao *dao)
{
	m_dao = dao;
}

void UserServiceImpl::doCreate(User &u)
{
	m_dao->create(u);
}

bool UserServiceImpl::doFetch(User &u)
{
	return m_dao->fetch(u);
}
