#ifndef BEEEON_AUTH_SERVICE_IMPL_H
#define BEEEON_AUTH_SERVICE_IMPL_H

#include <map>
#include <vector>

#include "dao/IdentityDao.h"
#include "dao/UserDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "l10n/LocaleManager.h"
#include "provider/AuthProvider.h"
#include "server/SessionManager.h"
#include "server/Session.h"
#include "service/IdentityListener.h"
#include "service/AuthService.h"
#include "transaction/Transactional.h"
#include "util/EventSource.h"

namespace BeeeOn {

/**
 * Template class for authentication by using various providers.
 * A user login operation is performed by utilizing AuthProviders.
 * This makes possible to choose the provider by user.
 */
class AuthServiceImpl : public AuthService, public Transactional {
public:
	typedef std::map<const std::string, AuthProvider *> Providers;

	AuthServiceImpl()
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
			std::make_pair(provider->id(), provider));
	}

	void setLocaleManager(LocaleManager::Ptr manager)
	{
		m_localeManager = manager;
	}

	void registerListener(IdentityListener::Ptr listener);
	void setEventsExecutor(AsyncExecutor::Ptr executor);

	const Session::Ptr login(const Credentials &cred);
	void logout(const std::string &id);

	void list(std::vector<AuthProvider *> &providers);

protected:
	Session::Ptr loginAuthorized(const AuthResult &result)
	{
		return BEEEON_TRANSACTION_RETURN(
			Session::Ptr,
			doLoginAuthorized(result)
		);
	}

	Session::Ptr doLoginAuthorized(const AuthResult &result);

	Session::Ptr openSession(const VerifiedIdentity &verifiedIdentity);
	Session::Ptr verifyIdentityAndLogin(const AuthResult &result);
	Session::Ptr loginAsNew(const AuthResult &result);
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
	LocaleManager::Ptr m_localeManager;
	EventSource<IdentityListener> m_eventSource;
};

}

#endif
