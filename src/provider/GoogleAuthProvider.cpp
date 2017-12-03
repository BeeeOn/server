#include <Poco/Logger.h>
#include <Poco/URI.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Net/HTMLForm.h>

#include "di/Injectable.h"
#include "ssl/SSLClient.h"
#include "provider/GoogleAuthProvider.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GoogleAuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_CASTABLE(OAuth2AuthProvider)
BEEEON_OBJECT_TEXT("clientId", &GoogleAuthProvider::setClientId)
BEEEON_OBJECT_TEXT("clientSecret", &GoogleAuthProvider::setClientSecret)
BEEEON_OBJECT_TEXT("redirectUri", &GoogleAuthProvider::setRedirectURI)
BEEEON_OBJECT_TEXT("id", &GoogleAuthProvider::setID)
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
	Object::Ptr info = JsonUtil::parse(userInfo);

	if (info->has("sub"))
		result.setProviderID(info->getValue<string>("sub"));

	if (info->has("email"))
		result.setEmail(Sanitize::email(info->getValue<string>("email")));

	if (result.email().empty() || result.providerID().empty())
		return false;

	if (info->has("given_name"))
		result.setFirstName(Sanitize::common(info->getValue<string>("given_name")));
	if (info->has("family_name"))
		result.setLastName(Sanitize::common(info->getValue<string>("family_name")));
	if (info->has("picture"))
		result.setPicture(Sanitize::uri(info->getValue<string>("picture")));
	if (info->has("locale"))
		result.setLocale(info->getValue<string>("locale"));

	result.setAccessToken(tokens.accessToken);

	return true;
}

bool GoogleAuthProvider::verifyAuthCode(const string &authCode, AuthResult &info)
{
	GoogleTokens tokens;
	string rawInfo;
	string google_id;

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
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	form.set("code", authCode);
	form.set("redirect_uri", m_redirectURI);
	form.set("client_id", m_clientId);
	form.set("client_secret", m_clientSecret);
	form.set("scope", ""); // No need to specify, defaults to userinfo.profile,userinfo.email,userinfo.locale
	form.set("grant_type", "authorization_code");

	URI host(m_tokenUrl);
	string receiveResponse = makeRequest(HTTPRequest::HTTP_POST, host, form);

	Object::Ptr object = JsonUtil::parse(receiveResponse);

	if (object->has("error")) {
		string message = object->getValue<string>("error_description");
		throw NotAuthenticatedException(message);
	}

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
	if (tokens.idToken.empty())
		throw NotAuthenticatedException("missing id_token");

	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	form.set("id_token", tokens.idToken);
	URI host(m_tokenInfoUrl);

	return makeRequest(HTTPRequest::HTTP_GET, host, form);
}
