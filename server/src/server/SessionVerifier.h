#ifndef BEEEON_SESSION_VERIFIER_H
#define BEEEON_SESSION_VERIFIER_H

#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>

#include "di/InjectorTarget.h"
#include "server/Session.h"
#include "server/SessionManager.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Abstract class providing access to sessions.
 */
class SessionVerifier : public AbstractInjectorTarget {
public:
	SessionVerifier();

	/**
	 * Verify the session exists and return it.
	 */
	template <typename Request>
	ExpirableSession::Ptr verifyAuthorized(const Request &request)
	{
		_TRACE_METHOD(m_logger);

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

protected:
	Poco::Logger &m_logger;
	SessionManager *m_sessionManager;
};

}

#endif
