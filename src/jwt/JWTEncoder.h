#pragma once

#include <jwt.h>

#include <vector>

#include <Poco/SharedPtr.h>

#include "model/TokenID.h"
#include "util/Loggable.h"

namespace BeeeOn {

class JWToken;

/**
 * @brief Encode the given instances of JWToken into the string
 * JWT representation. The result is signed by the preset algorithm
 * and appropriate secret passphrase.
 */
class JWTEncoder : public Loggable {
public:
	typedef Poco::SharedPtr<JWTEncoder> Ptr;

	JWTEncoder();
	~JWTEncoder();

	/**
	 * Set secret for signing tokens.
	 */
	void setSecret(const std::string &secret);

	/**
	 * Set algorithm to be used when signing tokens.
	 * Possible values:
	 * - "HS256"
	 * - "HS384"
	 * - "HS512"
	 * - "RS256"
	 * - "RS384"
	 * - "RS512"
	 * - "NONE"
	 */
	void setAlgorithm(const std::string &algorithm);

	/**
	 * Encode the token into JWT payload, prepend header
	 * and generate a signature according to the given
	 * settings.
	 */
	TokenID encode(const JWToken &token) const;

private:
	std::string m_secret;
	jwt_alg_t m_algorithm;
};
}
