#ifndef BEEEON_USER_DAO_H
#define BEEEON_USER_DAO_H

#include <Poco/Exception.h>

#include "di/AbstractInjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/User.h"
#include "model/Identity.h"

namespace BeeeOn {

class UserDao {
public:
	virtual void create(User &user) = 0;
	virtual bool fetch(User &user) = 0;
};

class NullUserDao : public AbstractInjectorTarget,
		public NullDao<User, UserDao> {
};

class MockUserDao : public AbstractInjectorTarget,
		public MockDao<User, UserDao> {
protected:
	UserID nextID()
	{
		return UserID::random();
	}
};

}

#endif
