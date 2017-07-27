#ifndef BEEEON_USER_SERVICE_H
#define BEEEON_USER_SERVICE_H

#include "dao/UserDao.h"

namespace BeeeOn {

/**
 * User management service. Its purpose is to ensure creating
 * user profiles and operations strongly related to a user.
 */
class UserService {
public:
	virtual ~UserService();

	virtual void create(User &u) = 0;
	virtual bool fetch(User &u) = 0;
};

}

#endif
