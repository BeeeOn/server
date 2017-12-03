#ifndef BEEEON_SESSION_VERIFIER_H
#define BEEEON_SESSION_VERIFIER_H

#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>

#include "server/Session.h"
#include "server/SessionManager.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * Abstract class providing access to sessions.
 */
class SessionVerifier : public Loggable {
public:
	SessionVerifier();

	/**
	 * Verify the session exists and return it.
	 */
	template <typename Request>
	ExpirableSession::Ptr verifyAuthorized(const Request &request)
	{
		if (!request.hasCredentials()) {
			throw Poco::Net::NotAuthenticatedException(
					"missing credentials");
		}

		std::string scheme;
		std::string authInfo;
		request.getCredentials(scheme, authInfo);

		return verifyAuthorized(scheme, authInfo);
	}

	ExpirableSession::Ptr verifyAuthorized(
			const std::string &scheme,
			const std::string &authInfo);

	void setSessionManager(SessionManager *manager)
	{
		m_sessionManager = manager;
	}

private:
	ExpirableSession::Ptr doVerifyAuthorized(
			const std::string &scheme,
			const std::string &authInfo);

protected:
	SessionManager *m_sessionManager;
};

}

#endif
