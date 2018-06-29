#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

#include "provider/AuthProvider.h"
#include "oauth2/OAuth2CodeExchanger.h"

namespace BeeeOn {

/**
 * @brief OAuth2UserFetcher provides an interface for obtaining
 * user information based on a authorization tokens obtained from
 * another source.
 */
class OAuth2UserFetcher {
public:
	typedef Poco::SharedPtr<OAuth2UserFetcher> Ptr;

	virtual ~OAuth2UserFetcher();

	/**
	 * @brief Exchange the given authorization code for token.
	 */
	virtual void fetch(
		const OAuth2CodeExchanger::Tokens &tokens,
		AuthResult &result,
		const Poco::Timestamp &now = {}) = 0;

	/**
	 * @returns client ID of the OAuth2.
	 */
	virtual std::string clientId() const = 0;
};

}
