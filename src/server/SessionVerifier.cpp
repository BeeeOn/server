#include <Poco/Exception.h>
#include <Poco/String.h>

#include "di/Injectable.h"
#include "server/SessionVerifier.h"
#include "util/Sanitize.h"

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
	try {
		return doVerifyAuthorized(
			Sanitize::encoding(scheme),
			Sanitize::base64(authInfo, ".")
		);
	} catch (const NotAuthenticatedException &e) {
		e.rethrow();
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		throw NotAuthenticatedException("failed to authorize", e);
	} catch (const std::exception &e) {
		throw NotAuthenticatedException(
			"failed to authorize: " + std::string(e.what()));
	}

	throw NotAuthenticatedException("terribly failed to authorize");
}

ExpirableSession::Ptr SessionVerifier::doVerifyAuthorized(
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
