#ifndef BEEEON_USER_DAO_H
#define BEEEON_USER_DAO_H

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/User.h"

namespace BeeeOn {

class UserDao {
public:
	virtual UserID create(const User &user) = 0;
	virtual User::Ptr get(UserID userId) = 0;
	virtual User::Ptr getByEmail(const std::string &email) = 0;
};

class NullUserDao : public AbstractInjectorTarget,
		public NullDao<User, UserDao> {
public:
	User::Ptr getByEmail(const std::string &email)
	{
		throw Poco::NotImplementedException(__func__);
	}
};

class MockUserDao : public AbstractInjectorTarget,
		public MockDao<User, UserDao> {
public:
	User::Ptr getByEmail(const std::string &email)
	{
		Storage::iterator it;

		for (it = storage().begin(); it != storage().end(); ++it) {
			User::Ptr user = it->second;

			if (email.compare(user->email()) == 0)
				return user;
		}

		return NULL;
	}
};

}

#endif
