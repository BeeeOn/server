#include <sstream>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/InvalidCertificateHandler.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/NetException.h>
#include <Poco/StreamCopier.h>
#include <Poco/URI.h>

#include "net/HTTPUtil.h"
#include "provider/OAuth2AuthProvider.h"
#include "ssl/SSLClient.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;
using namespace Poco::Net;

OAuth2AuthProvider::OAuth2AuthProvider(const string &name):
		AbstractAuthProvider(name),
		m_sslConfig(0)
{
}

bool OAuth2AuthProvider::authorize(const Credentials &cred, AuthResult &result)
{
	if (typeid(cred) == typeid(AuthCodeCredentials)) {
		const AuthCodeCredentials &authCodeCredentials =
			static_cast<const AuthCodeCredentials &>(cred);

		result.setProvider(name());
		return verifyAuthCode(authCodeCredentials, result);
	}
	if (typeid(cred) == typeid(AccessTokenCredentials)) {
		const AccessTokenCredentials &accessTokenCredentials =
			dynamic_cast<const AccessTokenCredentials &>(cred);

		result.setProvider(name());
		return verifyAuthCode(accessTokenCredentials, result);
	}

	throw NotAuthenticatedException("unrecognized credentials type");
}

bool OAuth2AuthProvider::verifyAuthCode(const AuthCodeCredentials &, AuthResult &)
{
	throw NotImplementedException("auth-code capability is not implemented");
}

bool OAuth2AuthProvider::verifyAuthCode(const AccessTokenCredentials &, AuthResult &)
{
	throw NotImplementedException("access-token capability is not implemented");
}

void OAuth2AuthProvider::validateSSL()
{
	if (m_sslConfig.isNull())
		throw IllegalStateException(
				"missing sslConfig, cannot use OAuth");
}

string OAuth2AuthProvider::handleResponse(HTTPEntireResponse &response) const
{
	if (logger().debug()) {
		logger().debug("response status: "
			+ to_string(response.getStatus())
			+ " "
			+ response.getReason(), __FILE__, __LINE__);
	}

	if (logger().debug()) {
		logger().debug("response: "
			+ response.getBody(), __FILE__, __LINE__);
	}

	return response.getBody();
}

string OAuth2AuthProvider::makeRequest(const string &method, URI &host, HTMLForm &requestForm) const
{
	HTTPRequest request(method, host.getPathAndQuery(), HTTPMessage::HTTP_1_1);
	requestForm.prepareSubmit(request);

	ostringstream requestQuery;
	requestForm.write(requestQuery);

	if (logger().debug())
		logger().debug("request: " + requestQuery.str(), __FILE__, __LINE__);

	HTTPEntireResponse response = HTTPUtil::makeRequest(
		request, host, requestQuery.str(), m_sslConfig);

	return handleResponse(response);
}
