#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Logger.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/NetException.h>

#include "di/Injectable.h"
#include "oauth2/FacebookOAuth2Provider.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

BEEEON_OBJECT_BEGIN(BeeeOn, FacebookOAuth2Provider)
BEEEON_OBJECT_CASTABLE(OAuth2CodeExchanger)
BEEEON_OBJECT_CASTABLE(OAuth2UserFetcher)
BEEEON_OBJECT_PROPERTY("exchangeUrl", &FacebookOAuth2Provider::setExchangeUrl)
BEEEON_OBJECT_PROPERTY("clientId", &FacebookOAuth2Provider::setClientId)
BEEEON_OBJECT_PROPERTY("clientSecret", &FacebookOAuth2Provider::setClientSecret)
BEEEON_OBJECT_PROPERTY("redirectUri", &FacebookOAuth2Provider::setRedirectUri)
BEEEON_OBJECT_PROPERTY("userInfoUrl", &FacebookOAuth2Provider::setFetchUrl)
BEEEON_OBJECT_PROPERTY("sslConfig", &FacebookOAuth2Provider::setSSLConfig)
BEEEON_OBJECT_END(BeeeOn, FacebookOAuth2Provider)

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace BeeeOn;

FacebookOAuth2Provider::FacebookOAuth2Provider():
	AbstractOAuth2Provider(
		HTTPClient(HTTPRequest::HTTP_POST,
			"https://graph.facebook.com/oauth/access_token"),
		HTTPClient(HTTPRequest::HTTP_GET,
			"https://graph.facebook.com/me")
	),
	m_inspector(HTTPRequest::HTTP_GET,
		"https://graph.facebook.com/debug_token")
{
}

void FacebookOAuth2Provider::setSSLConfig(SSLClient::Ptr config)
{
	AbstractOAuth2Provider::setSSLConfig(config);
	m_inspector.setSSLConfig(config);
}

OAuth2CodeExchanger::Tokens FacebookOAuth2Provider::parseTokens(
		const Object::Ptr body) const
{
	if (body->has("error")) {
		string message = body->getObject("error")->getValue<string>("message");
		throw NotAuthenticatedException(message);
	}

	return AbstractOAuth2Provider::parseTokens(body);
}

void FacebookOAuth2Provider::verifyAccessToken(
	const OAuth2CodeExchanger::Tokens &tokens) const
{
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	form.set("input_token", tokens.accessToken);
	form.set("access_token", clientId() + "|" + clientSecret());

	Object::Ptr body = JsonUtil::parse(m_inspector.requestBody(form));
	Object::Ptr data = body->getObject("data");

	if (data->has("error")) {
		string message = data->getObject("error")->getValue<string>("message");
		throw NotAuthenticatedException(message);
	}

	if (!data->optValue<bool>("is_valid", true))
		throw NotAuthenticatedException("access token marked as invalid");

	const auto &appId = data->getValue<string>("app_id");
	if (appId != clientId())
		throw NotAuthenticatedException("access token has unexpected app_id: " + appId);

	const auto expiresAt = Timestamp::fromEpochTime(data->getValue<long>("expires_at"));
	if (expiresAt.elapsed() > 0) {
		throw NotAuthenticatedException("access token has expired at : "
			+ DateTimeFormatter::format(expiresAt, DateTimeFormat::ISO8601_FORMAT));
	}

	if (logger().debug()) {
		logger().debug(
			"access_token has been verified: "
			+ data->optValue<string>("type", "<no-type>") + ", "
			+ data->optValue<string>("application", "<no-application>") + ", "
			+ to_string(data->getValue<long>("expires_at")) + ", "
			+ to_string(data->getValue<long>("issued_at")) + ", "
			+ data->optValue<string>("user_id", "<no-user>"),
			__FILE__, __LINE__);
	}
}

void FacebookOAuth2Provider::prepareFetch(
		HTMLForm &form,
		const OAuth2CodeExchanger::Tokens &tokens) const
{
	if (tokens.tokenType != "bearer") {
		throw InvalidArgumentException("incompatible token type: "
				+ tokens.tokenType);
	}

	verifyAccessToken(tokens);

	form.set("access_token", tokens.accessToken);
	form.set("fields", "name,picture,email,first_name,last_name,locale");
}

void FacebookOAuth2Provider::parseUserInfo(
		const Object::Ptr info,
		AuthResult &result) const
{
	if (info->has("error")) {
		string message = info->getObject("error")->getValue<string>("message");
		throw NotAuthenticatedException(message);
	}

	if (info->has("id"))
		result.setProviderID(info->getValue<string>("id"));

	if (info->has("picture")) {
		Object::Ptr pictData = info->getObject("picture");
		if (pictData->has("data")) {
			Object::Ptr picture  = pictData->getObject("data");
			if (picture->has("url"))
				result.setPicture(Sanitize::uri(picture->getValue<string>("url")));
		}
	}

	return AbstractOAuth2Provider::parseUserInfo(info, result);
}
