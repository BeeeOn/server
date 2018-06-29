#include <Poco/Logger.h>
#include <Poco/Net/HTMLForm.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/NetException.h>

#include "oauth2/AbstractOAuth2Provider.h"
#include "util/JsonUtil.h"
#include "util/Sanitize.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Net;
using namespace BeeeOn;

AbstractOAuth2Provider::AbstractOAuth2Provider(
		const HTTPClient &exchanger,
		const HTTPClient &fetcher):
	m_exchanger(exchanger),
	m_fetcher(fetcher),
	m_redirectUri("http://localhost")
{
}

void AbstractOAuth2Provider::setExchangeUrl(const string &url)
{
	m_exchanger.setURL(URI(url));
}

void AbstractOAuth2Provider::setRedirectUri(const string &uri)
{
	m_redirectUri = uri;
}

void AbstractOAuth2Provider::setClientId(const string &id)
{
	m_clientId = id;
}

void AbstractOAuth2Provider::setClientSecret(const string &secret)
{
	m_clientSecret = secret;
}

void AbstractOAuth2Provider::setFetchUrl(const string &url)
{
	m_fetcher.setURL(URI(url));
}

void AbstractOAuth2Provider::setSSLConfig(SSLClient::Ptr config)
{
	m_exchanger.setSSLConfig(config);
	m_fetcher.setSSLConfig(config);
}

string AbstractOAuth2Provider::redirectUri() const
{
	return m_redirectUri;
}

string AbstractOAuth2Provider::clientId() const
{
	return m_clientId;
}

string AbstractOAuth2Provider::clientSecret() const
{
	return m_clientSecret;
}

void AbstractOAuth2Provider::prepareExchange(
		HTMLForm &form,
		const AuthCodeCredentials &cred) const
{
	form.set("code", cred.authCode());
	form.set("redirect_uri", m_redirectUri);
	form.set("client_id", m_clientId);
	form.set("client_secret", m_clientSecret);
}

OAuth2CodeExchanger::Tokens AbstractOAuth2Provider::parseTokens(
		const Object::Ptr body) const
{
	return OAuth2CodeExchanger::extractTokens(body);
}

OAuth2CodeExchanger::Tokens AbstractOAuth2Provider::exchange(
		const AuthCodeCredentials &cred)
{
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	prepareExchange(form, cred);

	if (logger().debug()) {
		logger().debug(
			"exchanging credentials for tokens",
			__FILE__, __LINE__);
	}

	Object::Ptr body = JsonUtil::parse(m_exchanger.requestBody(form));
	return parseTokens(body);
}

void AbstractOAuth2Provider::parseUserInfo(
		const Object::Ptr info,
		AuthResult &result) const
{
	if (info->has("sub"))
		result.setProviderID(info->getValue<string>("sub"));
	if (info->has("email"))
		result.setEmail(Sanitize::email(info->getValue<string>("email")));
	if (info->has("given_name"))
		result.setFirstName(Sanitize::common(info->getValue<string>("given_name")));
	if (info->has("family_name"))
		result.setLastName(Sanitize::common(info->getValue<string>("family_name")));
	if (info->has("locale"))
		result.setLocale(Sanitize::locale(info->getValue<string>("locale")));
}

void AbstractOAuth2Provider::fetch(
	const OAuth2CodeExchanger::Tokens &tokens,
	AuthResult &result,
	const Timestamp &)
{
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	prepareFetch(form, tokens);

	if (logger().debug()) {
		logger().debug(
			"fetching user info based on tokens",
			__FILE__, __LINE__);
	}

	Object::Ptr info = JsonUtil::parse(m_fetcher.requestBody(form));
	parseUserInfo(info, result);

	result.setAccessToken(tokens.accessToken);

	if (result.providerID().empty())
		throw NotFoundException("providerID property has not been set (BUG?)");
	if (result.email().empty())
		throw NotFoundException("email property has not been set (BUG?)");
}
