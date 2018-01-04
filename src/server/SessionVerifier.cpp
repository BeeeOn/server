#include <Poco/Logger.h>
#include <Poco/Exception.h>

#include "server/SessionVerifier.h"
#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

SessionVerifier::SessionVerifier(const string &scheme):
	m_scheme(scheme)
{
}

SessionVerifier::~SessionVerifier()
{
}

void SessionVerifier::setScheme(const string &scheme)
{
	m_scheme = scheme;
}

string SessionVerifier::scheme() const
{
	return m_scheme;
}

Session::Ptr SessionVerifier::verifyAuthorized(
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
