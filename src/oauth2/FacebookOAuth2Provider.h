#pragma once

#include <Poco/SharedPtr.h>

#include "oauth2/AbstractOAuth2Provider.h"

namespace BeeeOn {

/**
 * @brief Implement the OAuth2UserFetcher and OAuth2CodeExchanger for Facebook.
 */
class FacebookOAuth2Provider :
	public AbstractOAuth2Provider {
public:
	typedef Poco::SharedPtr<FacebookOAuth2Provider> Ptr;

	/**
	 * @brief Setup the obvious settings that should work out-of-the-box
	 * except of client ID, client secret and SSL/TLS configuration.
	 */
	FacebookOAuth2Provider();

	/**
	 * @brief Overriden to propage the configuration to the m_inspector as well.
	 */
	void setSSLConfig(SSLClient::Ptr config) override;

protected:
	/**
	 * @brief Check for presence of fields 'error' and 'message' and throw
	 * NotAuthenticatedException if found. Otherwise, call the generic
	 * AbstractOAuth2Provider::parseTokens().
	 */
	OAuth2CodeExchanger::Tokens parseTokens(
		const Poco::JSON::Object::Ptr body) const override;

	/**
	 * @brief Perform online verification of the access token given in the
	 * set of tokens. The verification succeeds when the request to the
	 * '/debug_token' path returns the expected 'app_id' (client ID).
	 * Also, properties 'is_valid' and 'expires_at' are checked.
	 * If the verification fails, it throws NotAuthenticatedException.
	 */
	void verifyAccessToken(
		const OAuth2CodeExchanger::Tokens &tokens) const;

	/**
	 * @brief Check that we have the token type 'bearer', verify the
	 * access token and update the form with fields 'access_token' and 'fields'.
	 */
	void prepareFetch(
		Poco::Net::HTMLForm &form,
		const OAuth2CodeExchanger::Tokens &tokens) const override;

	/**
	 * @brief Check for presence of 'error' and 'message' fields to throw
	 * NotAuthenticatedException. Otherwise, process the 'picture' field
	 * and call AbstractOAuth2Provider::parseUserInfo().
	 */
	void parseUserInfo(
		const Poco::JSON::Object::Ptr info,
		AuthResult &result) const override;

private:
	/**
	 * The inspector is used to verify the given access token.
	 */
	HTTPClient m_inspector;
};

}
