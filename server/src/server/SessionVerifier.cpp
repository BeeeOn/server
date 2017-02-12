#include <Poco/String.h>

#include "server/SessionVerifier.h"
#include "Debug.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SessionVerifier::SessionVerifier()
{
	injector("sessionManager", &SessionVerifier::setSessionManager);
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
