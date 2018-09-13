#pragma once

#include "dao/UserDao.h"
#include "service/UserService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

/**
 * User management service. Its purpose is to ensure creating
 * user profiles and operations strongly related to a user.
 */
class UserServiceImpl : public UserService, public Transactional {
public:
	UserServiceImpl();

	void setUserDao(UserDao *dao);

	void create(User &u)
	{
		BEEEON_TRANSACTION(doCreate(u));
	}

	bool fetch(User &u)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(u));
	}

protected:
	void doCreate(User &u);
	bool doFetch(User &u);

private:
	UserDao *m_dao;
};

}
