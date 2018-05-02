#ifndef BEEEON_OAUTH2_AUTH_PROVIDER_H
#define BEEEON_OAUTH2_AUTH_PROVIDER_H

#include <Poco/SharedPtr.h>

#include "oauth2/OAuth2CodeExchanger.h"
#include "oauth2/OAuth2UserFetcher.h"
#include "provider/AuthProvider.h"

namespace BeeeOn {

/**
 * @brief OAuth2AuthProvider is a generic provider that implements the OAuth2
 * flow based on the OAuth2CodeExchanger and OAuth2UserFetcher instances.
 * This allows to easily add new OAuth2 identity providers by reusing the
 * existing code base as much as possible.
 *
 * If the codeExchanger property is not set, only access tokens are supported
 * for authentication if the given OAuth2UserFetcher supports exchange of
 * access tokens for user info.
 */
class OAuth2AuthProvider : public AuthProvider {
public:
	OAuth2AuthProvider();

	/**
	 * @brief Set ID as presented to users. This allows to choose the proper
	 * authorization flow by users. The ID should be unique among AuthProvider
	 * instances.
	 */
	void setId(const std::string &id);
	const std::string &id() const override;

	/**
	 * @brief Set name of the authorization method. The name do not have to be
	 * unique and sometimes, it is desirable that multiple AuthProvider instances
	 * have the same name. The name usually refers to the identity provider.
	 */
	void setName(const std::string &name);
	const std::string &name() const override;

	std::string clientId() const;
	std::string redirectURI() const;

	/**
	 * Set OAuth2CodeExchanger instance that adds support for
	 * the authorization code verification. Otherwise, only
	 * access tokens can be used.
	 */
	void setCodeExchanger(OAuth2CodeExchanger::Ptr exchanger);

	/**
	 * Set OAuth2UserFetcher that allows to fetch user info
	 * based on any of the given tokens (depends on what the
	 * given OAuth2UserFetcher supports).
	 */
	void setUserFetcher(OAuth2UserFetcher::Ptr fetcher);

	/**
	 * Configure token type that would be passed into the
	 * OAuth2UserFetcher for fetching user info. The value
	 * is usually "Bearer".
	 */
	void setTokenType(const std::string &type);

	/**
	 * Implement the OAuth2 authorization process for the given credentials.
	 * AuthCodeCredentials and AccessTokenCredentials are supported but they
	 * might not be supported by the configuration of OAuth2CodeExchanger and
	 * OAuth2UserFetcher.
	 */
	bool authorize(const Credentials &cred, AuthResult &result) override;

private:
	std::string m_id;
	std::string m_name;
	std::string m_tokenType;
	OAuth2CodeExchanger::Ptr m_exchanger;
	OAuth2UserFetcher::Ptr m_fetcher;
};

}

#endif
