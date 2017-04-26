#include <Poco/Logger.h>
#include <Poco/URI.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>

#include "Debug.h"
#include "di/Injectable.h"
#include "ssl/SSLClient.h"
#include "provider/GoogleAuthProvider.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GoogleAuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_CASTABLE(AuthCodeAuthProvider)
BEEEON_OBJECT_CASTABLE(OAuth2AuthProvider)
BEEEON_OBJECT_TEXT("client_id", &GoogleAuthProvider::setClientId)
BEEEON_OBJECT_TEXT("client_secret", &GoogleAuthProvider::setClientSecret)
BEEEON_OBJECT_TEXT("redirect_uri", &GoogleAuthProvider::setRedirectURI)
BEEEON_OBJECT_REF("sslConfig", &GoogleAuthProvider::setSSLConfig)
BEEEON_OBJECT_END(BeeeOn, GoogleAuthProvider)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace BeeeOn;

bool GoogleAuthProvider::parseIdentity(const std::string &userInfo,
		const GoogleTokens &tokens,
		AuthResult &result)
{
	Parser parser;
	Object::Ptr info = parser.parse(userInfo).extract<Object::Ptr>();

	if (info->has("sub"))
		result.setProviderID(info->getValue<string>("sub"));

	if (info->has("email"))
		result.setEmail(info->getValue<string>("email"));

	if (result.email().empty() || result.providerID().empty())
		return false;

	if (info->has("given_name"))
		result.setFirstName(info->getValue<string>("given_name"));
	if (info->has("family_name"))
		result.setLastName(info->getValue<string>("family_name"));
	if (info->has("picture"))
		result.setPicture(info->getValue<string>("picture"));

	result.setAccessToken(tokens.idToken);

	return true;
}

bool GoogleAuthProvider::verifyAuthCode(const string &authCode, AuthResult &info)
{
	GoogleTokens tokens;
	string rawInfo;
	string google_id;
	string email;

	try {
		tokens = requestTokens(authCode);

		if (tokens.tokenType != "Bearer")
			throw NotAuthenticatedException("incompatible token type: " + tokens.tokenType);
		if (tokens.accessToken.empty())
			throw NotAuthenticatedException("missing access_token");
		if (tokens.expiresIn == 0)
			throw NotAuthenticatedException("token has expired");

		rawInfo = fetchUserInfo(tokens);
	} catch(const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		return false;
	}

	return parseIdentity(rawInfo, tokens, info);
}

GoogleAuthProvider::GoogleTokens GoogleAuthProvider::requestTokens(const string &authCode)
{
	TRACE_METHOD();

	SharedPtr<HTTPSClientSession> session;
	URI uri(m_tokenUrl);

	session = connectSecure(uri.getHost(), uri.getPort());

	string requestRaw = "code=" + authCode + "&"
		"redirect_uri=" + m_redirectURI + "&"
		"client_id=" + m_clientId + "&"
		"client_secret=" + m_clientSecret + "&"
		"scope=&"	// No need to specify, defaults to userinfo.profile,userinfo.email
		"grant_type=authorization_code";

	if (logger().debug())
		logger().debug("request: " + requestRaw, __FILE__, __LINE__);

	HTTPRequest req(HTTPRequest::HTTP_POST,
			uri.getPathAndQuery(),
			HTTPMessage::HTTP_1_1);
	req.setContentType("application/x-www-form-urlencoded");
	req.setContentLength(requestRaw.length());

	session->sendRequest(req) << requestRaw;
	string receiveResponse = handleResponse(*session);

	Parser parser;
	Object::Ptr object = parser.parse(receiveResponse)
			.extract<Object::Ptr>();

	GoogleTokens tokens;

	tokens.accessToken = object->optValue<string>("access_token", "");
	tokens.refreshToken = object->optValue<string>("refresh_token", "");
	tokens.expiresIn = Timespan(object->optValue<unsigned int>("expires_in", 0), 0);
	tokens.tokenType = object->optValue<string>("token_type", "");
	tokens.idToken = object->optValue<string>("id_token", "");

	return tokens;
}

string GoogleAuthProvider::fetchUserInfo(const GoogleTokens &tokens)
{
	TRACE_METHOD();

	if (tokens.idToken.empty())
		throw NotAuthenticatedException("missing id_token");

	URI uri(m_tokenInfoUrl + tokens.idToken);
	SharedPtr<HTTPSClientSession> session;

	session = connectSecure(uri.getHost(), uri.getPort());

	HTTPRequest req(HTTPRequest::HTTP_GET,
			uri.getPathAndQuery(),
			HTTPMessage::HTTP_1_1);

	session->sendRequest(req);
	return handleResponse(*session);
}
