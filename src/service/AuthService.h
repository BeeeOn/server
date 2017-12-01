#ifndef BEEEON_AUTH_SERVICE_H
#define BEEEON_AUTH_SERVICE_H

#include <map>
#include <vector>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/SharedPtr.h>

#include "dao/IdentityDao.h"
#include "dao/UserDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "provider/AuthProvider.h"
#include "server/SessionManager.h"
#include "server/Session.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

/**
 * Service class for authentication by using various providers.
 * A user login operation is performed by utilizing AuthProviders.
 * This makes possible to choose the provider by user.
 */
class AuthService {
public:
	typedef Poco::SharedPtr<AuthService> Ptr;

	virtual ~AuthService();

	virtual const Session::Ptr login(const Credentials &cred) = 0;
	virtual void logout(const std::string &id) = 0;
	virtual void list(std::vector<AuthProvider *> &providers) = 0;
};

}

#endif
