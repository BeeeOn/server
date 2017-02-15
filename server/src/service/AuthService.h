#ifndef BEEEON_AUTH_SERVICE_H
#define BEEEON_AUTH_SERVICE_H

#include <map>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include "provider/AuthProvider.h"
#include "notification/NotificationDispatcher.h"
#include "dao/UserDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "dao/Transactional.h"
#include "server/SessionManager.h"
#include "server/Session.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Template class for authentication by using various providers.
 * A user login operation is performed by utilizing AuthProviders.
 * This makes possible to choose the provider by user.
 */
class AuthService : public Transactional {
public:
	typedef std::map<const std::string, AuthProvider *> Providers;

	AuthService()
	{
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

	void setNotificationDispatcher(NotificationDispatcher *service)
	{
		m_notificationService = service;
	}

	const ExpirableSession::Ptr login(const Credentials &cred);

	void logout(const std::string &id);

protected:
	ExpirableSession::Ptr loginAuthorized(const AuthResult &result)
	{
		return BEEEON_TRANSACTION_RETURN(
			ExpirableSession::Ptr,
			doLoginAuthorized(result)
		);
	}

	ExpirableSession::Ptr doLoginAuthorized(const AuthResult &result);

	ExpirableSession::Ptr openSession(const VerifiedIdentity &verifiedIdentity);
	ExpirableSession::Ptr verifyIdentityAndLogin(const AuthResult &result);
	ExpirableSession::Ptr loginAsNew(const AuthResult &result);
	void verifyIdentity(VerifiedIdentity &verifiedIdentity,
			Identity &identity, const AuthResult &result);
	void approveIdentity(VerifiedIdentity &verifiedIdentity,
		const Identity &identity, const User &user, const AuthResult &result);
	void createUserAndVerify(VerifiedIdentity &verifiedIdentity,
			Identity &identity, const AuthResult &result);
	User createUser(const AuthResult &result);

private:
	SessionManager *m_sessionManager;
	UserDao *m_userDao;
	IdentityDao *m_identityDao;
	VerifiedIdentityDao *m_verifiedIdentityDao;
	Providers m_providers;
	NotificationDispatcher *m_notificationService;
};

}

#endif
