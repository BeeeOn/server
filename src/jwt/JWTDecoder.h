#pragma once

#include <vector>

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>

#include "jwt/JWToken.h"
#include "util/Loggable.h"

namespace BeeeOn {

class TokenID;

/**
 * @brief JWTDecoder is used to decode JSON Web Token string into
 * its internal representation (an instance of JWToken).
 */
class JWTDecoder : public Loggable {
public:
	typedef Poco::SharedPtr<JWTDecoder> Ptr;

	JWTDecoder();
	~JWTDecoder();

	/**
	 * Set secret passphrase to be used while verifying the
	 * JWT signatures.
	 */
	void setSecret(const std::string &secret);

	/**
	 * Decode the given token, check its signature according to
	 * the preset secret. Any algorithm could have been used for
	 * signing the token, however, the secret must match.
	 */
	JWToken decode(const TokenID &tokenId) const;

private:
	std::string m_secret;
};

}
