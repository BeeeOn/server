#include "server/RestAuthHandler.h"

#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Object.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace Poco::JSON;
using namespace BeeeOn;

RestAuthHandler::RestAuthHandler()
{
	injector("authService", &RestAuthHandler::setAuthService);
}

void RestAuthHandler::extractAuthData(std::istream &in,
		std::string &provider, std::string &authCode)
{
	string data;
	StreamCopier::copyToString(in, data);

	Parser parser;
	Object::Ptr object;

	try {
		object = parser.parse(data).extract<Object::Ptr>();
	} catch (const JSONException &e) {
		logger().log(e, __FILE__, __LINE__);

		if (logger().debug()) {
			logger().debug("JSON dump: '" + data + "'",
					__FILE__, __LINE__);
		}

		throw NotAuthenticatedException("JSON auth data invalid");
	}

	if (!object->has("provider")) {
		throw NotAuthenticatedException("Missing field 'provider'");
	}

	if (!object->has("authCode")) {
		throw NotAuthenticatedException("Missing field 'authCode'");
	}

	provider = object->getValue<string>("provider");
	authCode = object->getValue<string>("authCode");
}

SessionID RestAuthHandler::handleLogin(istream &in)
{
	string provider;
	string authCode;

	extractAuthData(in, provider, authCode);
	const AuthCodeCredentials credentials(provider, authCode);

	const ExpirableSession::Ptr session =
		m_authService->login(credentials);

	if (session.isNull())
		throw NotAuthenticatedException("No session returned on login");

	return session->sessionID();
}

BEEEON_OBJECT(RestAuthHandler, BeeeOn::RestAuthHandler)
