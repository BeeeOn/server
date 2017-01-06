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
	UserService();

	void setUserDao(UserDao *dao);

	void create(User &u);

	bool fetch(User &u);

private:
	UserDao *m_dao;
};

}

#endif
