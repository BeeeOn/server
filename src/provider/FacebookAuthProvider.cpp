#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Object.h>
#include <Poco/Logger.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPSClientSession.h>
#include <Poco/URI.h>

#include "di/Injectable.h"
#include "ssl/SSLClient.h"
#include "provider/FacebookAuthProvider.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, FacebookAuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_CASTABLE(OAuth2AuthProvider)
BEEEON_OBJECT_REF("sslConfig", &FacebookAuthProvider::setSSLConfig)
BEEEON_OBJECT_TEXT("clientId", &FacebookAuthProvider::setClientId)
BEEEON_OBJECT_TEXT("clientSecret", &FacebookAuthProvider::setClientSecret)
BEEEON_OBJECT_TEXT("redirectUri", &FacebookAuthProvider::setRedirectURI)
BEEEON_OBJECT_TEXT("id", &FacebookAuthProvider::setID)
BEEEON_OBJECT_HOOK("done", &FacebookAuthProvider::validateSSL)
BEEEON_OBJECT_END(BeeeOn, FacebookAuthProvider)

using namespace BeeeOn;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace std;

bool FacebookAuthProvider::verifyAuthCode(
		const AuthCodeCredentials &credentials,
		AuthResult &info)
{
	string accessToken;

	try {
		accessToken = requestAccessToken(credentials.authCode());
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		return false;
	}

	return processAccessToken(accessToken, info);
}

bool FacebookAuthProvider::verifyAuthCode(
		const AccessTokenCredentials &credentials,
		AuthResult &info)
{
	return processAccessToken(credentials.accessToken(), info);
}

string FacebookAuthProvider::requestAccessToken(const string &authCode) const
{
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	form.set("client_id", m_clientId);
	form.set("redirect_uri", m_redirectURI);
	form.set("client_secret", m_clientSecret);
	form.set("code", authCode);

	URI host(m_requestTokenUrl);
	string response = makeRequest(HTTPRequest::HTTP_POST, host, form);
	Object::Ptr data = JsonUtil::parse(response);

	if (data->has("error")) {
		string message = data->getObject("error")->getValue<string>("message");
		throw NotAuthenticatedException(message);
	}

	string accessToken = data->optValue<string>("access_token", "");
	string type = data->optValue<string>("token_type", "");
	unsigned int expires = data->optValue<unsigned int>("expires_in", 0);

	if (accessToken.empty())
		throw NotAuthenticatedException("missing access_token");
	if (type != "bearer")
		throw NotAuthenticatedException("incompatible token type: " +  type);
	if (expires == 0)
		throw NotAuthenticatedException("token has expired");

	return accessToken;
}

bool FacebookAuthProvider::processAccessToken(const string &accessToken, AuthResult &info) const
{
	string userData;

	try {
		verifyAccessToken(accessToken);
		userData = fetchUserData(accessToken);
	} catch (const Exception &e) {
		logger().log(e, __FILE__, __LINE__);
		return false;
	}

	return parseIdentity(userData, accessToken, info);
}

void FacebookAuthProvider::verifyAccessToken(const string& accessToken) const
{
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	form.set("input_token", accessToken);
	form.set("access_token", m_clientId + "|" + m_clientSecret);

	URI host(m_inspectTokenUrl);
	string response = makeRequest(HTTPRequest::HTTP_GET, host, form);
	Object::Ptr data = JsonUtil::parse(response)->getObject("data");

	if (data->has("error")) {
		string message = data->getObject("error")->getValue<string>("message");
		throw NotAuthenticatedException(message);
	}

	string appId = data->getValue<string>("app_id");

	if (appId != m_clientId)
		throw NotAuthenticatedException("Unexpected app_id: " + appId);

}

string FacebookAuthProvider::fetchUserData(const string &accessToken) const
{
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	form.set("access_token", accessToken);
	form.set("fields", "name,picture,email,first_name,last_name,locale");

	URI host(m_userInfoUrl);
	string response = makeRequest(HTTPRequest::HTTP_POST, host, form);
	Object::Ptr data = JsonUtil::parse(response);

	if (data->has("error"))	{
		string message = data->getObject("error")->getValue<string>("message");
		throw NotAuthenticatedException(message);
	}

	return response;
}

bool FacebookAuthProvider::parseIdentity(const string &userInfo, const string &accessToken, AuthResult &result) const
{
	Object::Ptr info = JsonUtil::parse(userInfo);

	if (info->has("id"))
		result.setProviderID(info->getValue<string>("id"));

	if (info->has("email"))
		result.setEmail(Sanitize::email(info->getValue<string>("email")));

	if (info->has("first_name"))
		result.setFirstName(Sanitize::common(info->getValue<string>("first_name")));

	if (info->has("last_name"))
		result.setLastName(Sanitize::common(info->getValue<string>("last_name")));

	if (info->has("locale"))
		result.setLocale(Sanitize::locale(info->getValue<string>("locale")));

	if (info->has("picture")) {
		Object::Ptr pictData = info->getObject("picture");
		if (pictData->has("data")) {
			Object::Ptr picture  = pictData->getObject("data");
			if (picture->has("url"))
				result.setPicture(Sanitize::uri(picture->getValue<string>("url")));
		}
	}

	result.setAccessToken(accessToken);

	if (result.email().empty())
		throw NotAuthenticatedException("missing email");

	if (result.providerID().empty())
		throw NotAuthenticatedException("missing user_id");

	return true;

}
