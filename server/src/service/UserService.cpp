#include "service/UserService.h"

BEEEON_OBJECT(UserService, BeeeOn::UserService)

UserService::UserService()
{
	injector<UserService, UserDao>("userDao",
			&UserService::setUserDao);
}

void UserService::setUserDao(UserDao *dao)
{
	m_dao = dao;
}

void UserService::create(User &u)
{
	TRACE_METHOD();
	m_dao->create(u);
}

bool UserService::fetch(User &u)
{
	TRACE_METHOD();
	return m_dao->fetch(u);
}
