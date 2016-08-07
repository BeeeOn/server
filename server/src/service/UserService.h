#ifndef BEEEON_USER_SERVICE_H
#define BEEEON_USER_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "dao/UserDao.h"
#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * User management service. Its purpose is to ensure creating
 * user profiles and operations strongly related to a user.
 */
class UserService : public AbstractInjectorTarget {
public:
	UserService()
	{
		injector<UserService, UserDao>("userDao",
				&UserService::setUserDao);
	}

	void setUserDao(UserDao *dao)
	{
		m_dao = dao;
	}

	User::Ptr create(User &u)
	{
		TRACE_METHOD();
		UserID id = m_dao->create(u);
		return m_dao->get(id);
	}

	User::Ptr get(UserID id)
	{
		TRACE_METHOD();
		return m_dao->get(id);
	}

private:
	UserDao *m_dao;
};

}

#endif
