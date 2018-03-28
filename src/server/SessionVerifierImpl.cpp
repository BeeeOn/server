#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "server/SessionVerifierImpl.h"

BEEEON_OBJECT_BEGIN(BeeeOn, SessionVerifierImpl)
BEEEON_OBJECT_CASTABLE(SessionVerifier)
BEEEON_OBJECT_PROPERTY("sessionManager", &SessionVerifierImpl::setSessionManager)
BEEEON_OBJECT_PROPERTY("scheme", &SessionVerifierImpl::setScheme)
BEEEON_OBJECT_END(BeeeOn, SessionVerifierImpl)

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

Session::Ptr SessionVerifierImpl::doVerifyAuthorized(
		const std::string &authInfo)
{
	Session::Ptr session;

	if (m_sessionManager->lookup(authInfo, session))
		return session;

	throw NotAuthenticatedException("missing a session");
}
