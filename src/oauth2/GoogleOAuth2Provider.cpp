#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "oauth2/GoogleOAuth2Provider.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GoogleOAuth2Provider)
BEEEON_OBJECT_CASTABLE(OAuth2CodeExchanger)
BEEEON_OBJECT_CASTABLE(OAuth2UserFetcher)
BEEEON_OBJECT_PROPERTY("exchangeUrl", &GoogleOAuth2Provider::setExchangeUrl)
BEEEON_OBJECT_PROPERTY("clientId", &GoogleOAuth2Provider::setClientId)
BEEEON_OBJECT_PROPERTY("clientSecret", &GoogleOAuth2Provider::setClientSecret)
BEEEON_OBJECT_PROPERTY("redirectUri", &GoogleOAuth2Provider::setRedirectUri)
BEEEON_OBJECT_PROPERTY("tokenInfoUrl", &GoogleOAuth2Provider::setFetchUrl)
BEEEON_OBJECT_PROPERTY("sslConfig", &GoogleOAuth2Provider::setSSLConfig)
BEEEON_OBJECT_END(BeeeOn, GoogleOAuth2Provider)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace BeeeOn;

GoogleOAuth2Provider::GoogleOAuth2Provider():
	AbstractOAuth2Provider(
		HTTPClient(HTTPRequest::HTTP_POST,
			"https://www.googleapis.com/oauth2/v4/token"),
		HTTPClient(HTTPRequest::HTTP_GET,
			"https://www.googleapis.com/oauth2/v3/tokeninfo")
	)
{
}

void GoogleOAuth2Provider::prepareExchange(
		HTMLForm &form,
		const AuthCodeCredentials &cred) const
{
	AbstractOAuth2Provider::prepareExchange(form, cred);
	form.set("scope", "");
	form.set("grant_type", "authorization_code");
}

OAuth2CodeExchanger::Tokens GoogleOAuth2Provider::parseTokens(
		const Object::Ptr body) const
{
	if (body->has("error")) {
		const string message = body->getValue<string>("error_description");
		throw NotAuthenticatedException(message);
	}

	return AbstractOAuth2Provider::parseTokens(body);
}


void GoogleOAuth2Provider::prepareFetch(
		HTMLForm &form,
		const OAuth2CodeExchanger::Tokens &tokens) const
{
	if (tokens.tokenType != "Bearer") {
		throw InvalidArgumentException("incompatible token type: "
				+ tokens.tokenType);
	}

	if (tokens.idToken.empty())
		throw InvalidArgumentException("no id_token given");

	form.set("id_token", tokens.idToken);
}

void GoogleOAuth2Provider::parseUserInfo(
		const Object::Ptr info,
		AuthResult &result) const
{
	if (info->has("error")) {
		const auto &message = info->getObject("error")
				->getValue<string>("error_description");
		throw NotAuthenticatedException(message);
	}

	if (info->has("picture"))
		result.setPicture(URI(Sanitize::uri(info->getValue<string>("picture"))));

	return AbstractOAuth2Provider::parseUserInfo(info, result);
}
