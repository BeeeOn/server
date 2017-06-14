#ifndef BEEEON_USER_SERVICE_H
#define BEEEON_USER_SERVICE_H

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "dao/UserDao.h"
#include "transaction/Transactional.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * User management service. Its purpose is to ensure creating
 * user profiles and operations strongly related to a user.
 */
class UserService : public Transactional {
public:
	UserService();

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

#endif
