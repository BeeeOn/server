#ifndef BEEEON_USER_DAO_H
#define BEEEON_USER_DAO_H

#include <Poco/Exception.h>

#include "di/InjectorTarget.h"
#include "dao/NullDao.h"
#include "dao/MockDao.h"
#include "model/User.h"

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
public:
	bool fetch(User &user);
	bool has(const User &user);
	void create(User &user);
	bool update(User &user);
	bool remove(const User &user);

protected:
	Iterator findByEmail(const std::string &email);

	UserID nextID()
	{
		return ++m_id;
	}

private:
	UserID m_id;
};

}

#endif
