#pragma once

#include <string>

#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>

#include "oauth2/OAuth2CodeExchanger.h"
#include "oauth2/OAuth2UserFetcher.h"
#include "util/HTTPClient.h"
#include "util/Loggable.h"
#include "ssl/SSLClient.h"

namespace BeeeOn {

/**
 * @brief AbstractOAuth2Provider provides a helper facility to implement
 * support for an OAuth2 authorization mechanism. It implements the two
 * interfaces OAuth2CodeExchanger and OAuth2UserFetcher intended to solve
 * the OAuth2 authorization step by step.
 *
 * Both the OAuth2CodeExchanger::exchange() and OAuth2UserFetcher::fetch()
 * are generically implemented in a customizable way. The only method to
 * be implemented is prepareFetch() if the generic settings fits the target
 * OAuth2 provider's needs. It is however highly recommended to override
 * also parseTokens() and parseUserInfo() to handle error messages.
 */
class AbstractOAuth2Provider :
	public OAuth2CodeExchanger,
	public OAuth2UserFetcher,
	protected Loggable {
public:
	AbstractOAuth2Provider(
		const HTTPClient &exchanger,
		const HTTPClient &fetcher
	);

	void setRedirectUri(const std::string &uri);
	void setClientId(const std::string &id);
	void setClientSecret(const std::string &secret);
	void setExchangeUrl(const std::string &url);
	void setFetchUrl(const std::string &url);
	virtual void setSSLConfig(SSLClient::Ptr config);

	std::string clientId() const override;
	std::string redirectUri() const override;

	/**
	 * Exchange the given authorization code for access tokens via
	 * a HTTP request. The HTTP request is prepared by calling method
	 * prepareExchange() and the response body is processed by
	 * parseTokens().
	 */
	Tokens exchange(const AuthCodeCredentials &cred) override;

	/**
	 * Fetch user info by the given tokens via a HTTP request. Fill
	 * the given AuthResult instance with appropriate data. The HTTP
	 * request is prepared by calling method prepareFetch() and the
	 * response body is processed by parseUserInfo().
	 *
	 * The AuthResult instance *MUST* contain at least providerID and
	 * email otherwise the whole operation is considered as failed.
	 * The accessToken field is set automatically.
	 */
	void fetch(
		const OAuth2CodeExchanger::Tokens &tokens,
		AuthResult &result) override;

protected:
	std::string clientSecret() const;

	/**
	 * Prepare the given HTMLForm instance for the exchange HTTP request.
	 * The default implementation configures fields "code", "redirect_uri",
	 * "client_id" and "client_secret". The preselected HTTP encoding is
	 * HTMLForm::ENCODING_URL.
	 */
	virtual void prepareExchange(
		Poco::Net::HTMLForm &form,
		const AuthCodeCredentials &cred) const;

	/**
	 * Parse response received while exchanging credentials for tokens.
	 * The default implementation (optionally) extracts "access_token",
	 * "refresh_token", "id_token", "token_type" and "expires_in" fields.
	 * The "token_type" and "expires_in" fields are required.
	 */
	virtual OAuth2CodeExchanger::Tokens parseTokens(
		const Poco::JSON::Object::Ptr body) const;

	/**
	 * Prepare the HTMLForm instance for fetching user info based on the
	 * given tokens. As this method highly varies among different providers,
	 * there is no default implementation and thus this method must be
	 * overriden.
	 */
	virtual void prepareFetch(
		Poco::Net::HTMLForm &form,
		const OAuth2CodeExchanger::Tokens &tokens) const = 0;

	/**
	 * Parse response received while fetching user info. The default
	 * implementation optionally extracts fields "sub", "email",
	 * "given_name", "family_name" and "locale".
	 */
	virtual void parseUserInfo(
		const Poco::JSON::Object::Ptr info,
		AuthResult &result) const;

private:
	HTTPClient m_exchanger;
	HTTPClient m_fetcher;
	std::string m_redirectUri;
	std::string m_clientId;
	std::string m_clientSecret;
};

}

