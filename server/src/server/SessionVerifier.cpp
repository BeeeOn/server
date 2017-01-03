#include <Poco/String.h>

#include "server/SessionVerifier.h"
#include "Debug.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SessionVerifier::SessionVerifier():
	m_logger(LOGGER_CLASS(this))
{
	injector<SessionVerifier, SessionManager>("sessionManager",
			&SessionVerifier::setSessionManager);
}

ExpirableSession::Ptr SessionVerifier::verifyAuthorized(
		const std::string &scheme,
		const std::string &authInfo)
{
	if (icompare(scheme, "Bearer")) {
		throw NotAuthenticatedException(
				"unsupported scheme: " + scheme);
	}

	ExpirableSession::Ptr session;

	if (m_sessionManager->lookup(authInfo, session))
		return session;

	throw NotAuthenticatedException("missing a session");
}

BEEEON_OBJECT(SessionVerifier, BeeeOn::SessionVerifier)