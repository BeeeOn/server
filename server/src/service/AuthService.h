#ifndef BEEEON_AUTH_SERVICE_H
#define BEEEON_AUTH_SERVICE_H

#include <map>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "di/InjectorTarget.h"
#include "provider/AuthProvider.h"
#include "dao/UserDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "server/SessionManager.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Template class for authentication by using various providers.
 * A user login operation is performed by utilizing AuthProviders.
 * This makes possible to choose the provider by user.
 */
class AuthService : public AbstractInjectorTarget {
public:
	typedef std::map<const std::string, AuthProvider *> Providers;

	AuthService()
	{
		injector<AuthService, UserDao>("userDao",
				&AuthService::setUserDao);
		injector<AuthService, IdentityDao>("identityDao",
				&AuthService::setIdentityDao);
		injector<AuthService, VerifiedIdentityDao>("verifiedIdentityDao",
				&AuthService::setVerifiedIdentityDao);
		injector<AuthService, SessionManager>("sessionManager",
				&AuthService::setSessionManager);
		injector<AuthService, AuthProvider>("providers",
				&AuthService::registerProvider);
	}

	void setUserDao(UserDao *dao)
	{
		m_userDao = dao;
	}

	void setIdentityDao(IdentityDao *dao)
	{
		m_identityDao = dao;
	}

	void setVerifiedIdentityDao(VerifiedIdentityDao *dao)
	{
		m_verifiedIdentityDao = dao;
	}

	void setSessionManager(SessionManager *manager)
	{
		m_sessionManager = manager;
	}

	void registerProvider(AuthProvider *provider)
	{
		m_providers.insert(
			std::make_pair(provider->name(), provider));
	}

	const std::string login(const Credentials &cred);

	void logout(const std::string &id);

protected:
	std::string openSession(const VerifiedIdentity &verifiedIdentity);

private:
	SessionManager *m_sessionManager;
	UserDao *m_userDao;
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	Providers m_providers;
};

}

#endif
