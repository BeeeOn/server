#include <Poco/String.h>

#include "di/Injectable.h"
#include "server/SessionVerifier.h"
#include "Debug.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SessionVerifier::SessionVerifier()
{
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

BEEEON_OBJECT_BEGIN(BeeeOn, SessionVerifier)
BEEEON_OBJECT_REF("sessionManager", &SessionVerifier::setSessionManager)
BEEEON_OBJECT_END(BeeeOn, SessionVerifier)
