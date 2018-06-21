#pragma once

#include <Poco/SharedPtr.h>

#include "oauth2/AbstractOAuth2Provider.h"

namespace BeeeOn {

/**
 * @brief Implementation of OAuth2 provider for Google. It realizes
 * the two interfaces: OAuth2CodeExchanger and OAuth2UserFetcher.
 */
class GoogleOAuth2Provider : public AbstractOAuth2Provider {
public:
	typedef Poco::SharedPtr<GoogleOAuth2Provider> Ptr;

	/**
	 * @brief Construct the GoogleOAuth2Provider with default settings
	 * that should work out-of-the-box except of client ID, client secret
	 * and the SSL/TLS configuration.
	 */
	GoogleOAuth2Provider();

protected:
	/**
	 * @brief Set grant_type = 'authorization_code' into the form.
	 * The AbstractOAuth2Provider::prepareExchange() is called from
	 * inside.
	 */
	void prepareExchange(
		Poco::Net::HTMLForm &form,
		const AuthCodeCredentials &cred) const override;

	/**
	 * @brief Check for fields 'error' and 'error_description' and throw
	 * NotAuthenticatedException if they are present. Otherwise, the
	 * AbstractOAuth2Provider::parseTokens() is called.
	 */
	OAuth2CodeExchanger::Tokens parseTokens(
		const Poco::JSON::Object::Ptr body) const override;

	/**
	 * @brief Check we are receiving 'Bearer' token type and that the ID
	 * token was fetched. The ID token is set into the given form as 'id_token'.
	 */
	void prepareFetch(
		Poco::Net::HTMLForm &form,
		const OAuth2CodeExchanger::Tokens &tokens) const override;

	/**
	 * @brief Check for fields 'error' and 'error_description' and throw
	 * NotAuthenticatedException if they are present. Otherwise, extract
	 * parameter 'picture' from the response and call the generic method
	 * AbstractOAuth2Provider::parseUserInfo().
	 */
	void parseUserInfo(
		const Poco::JSON::Object::Ptr info,
		AuthResult &result) const override;
};

}
