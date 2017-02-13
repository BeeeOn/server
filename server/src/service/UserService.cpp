#include "service/UserService.h"

using namespace BeeeOn;

BEEEON_OBJECT(BeeeOn_UserService, BeeeOn::UserService)

UserService::UserService()
{
	injector("userDao", &UserService::setUserDao);
}

void UserService::setUserDao(UserDao *dao)
{
	m_dao = dao;
}

void UserService::doCreate(User &u)
{
	TRACE_METHOD();
	m_dao->create(u);
}

bool UserService::doFetch(User &u)
{
	TRACE_METHOD();
	return m_dao->fetch(u);
}
