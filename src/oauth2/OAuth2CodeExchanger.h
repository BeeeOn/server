#pragma once

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/JSON/Object.h>

#include "provider/AuthProvider.h"

namespace BeeeOn {

/**
 * @brief OAuth2CodeExchanger provides an interface for obtaining
 * tokens from a token endpoint for the given authorization code.
 */
class OAuth2CodeExchanger {
public:
	typedef Poco::SharedPtr<OAuth2CodeExchanger> Ptr;

	struct Tokens {
		std::string accessToken;
		std::string refreshToken;
		std::string idToken;
		std::string tokenType;
		Poco::Timespan expiresIn;
	};

	virtual ~OAuth2CodeExchanger();

	/**
	 * @returns redirect URI of the OAuth2 exchange.
	 */
	virtual std::string redirectUri() const = 0;

	/**
	 * @brief Exchange the given authorization code for token.
	 */
	virtual Tokens exchange(const AuthCodeCredentials &cred) = 0;

	/**
	 * @brief Generic extraction of token data out of a body of
	 * a (usually) HTTP response encoded in JSON format.
	 */
	static Tokens extractTokens(
		const Poco::JSON::Object::Ptr &body);
};

}
