#include <Poco/Dynamic/Var.h>
#include <Poco/Error.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/JSON/Object.h>

#include "di/Injectable.h"
#include "jwt/JWToken.h"
#include "jwt/JWTEncoder.h"

using namespace BeeeOn;
using namespace std;
using namespace Poco;
using namespace Poco::JSON;

BEEEON_OBJECT_BEGIN(BeeeOn, JWTEncoder)
BEEEON_OBJECT_PROPERTY("secret", &JWTEncoder::setSecret)
BEEEON_OBJECT_PROPERTY("algorithm", &JWTEncoder::setAlgorithm)
BEEEON_OBJECT_END(BeeeOn, JWTEncoder)

JWTEncoder::JWTEncoder():
	m_algorithm(JWT_ALG_NONE)
{
}

JWTEncoder::~JWTEncoder()
{
}

void JWTEncoder::setSecret(const std::string &secret)
{
	m_secret = secret;
}

void JWTEncoder::setAlgorithm(const std::string &algorithm)
{
	if (algorithm == "HS256")
		m_algorithm = JWT_ALG_HS256;
	else if (algorithm == "HS384")
		m_algorithm = JWT_ALG_HS384;
	else if (algorithm == "HS512")
		m_algorithm = JWT_ALG_HS512;
	else if (algorithm == "RS256")
		m_algorithm = JWT_ALG_RS256;
	else if (algorithm == "RS384")
		m_algorithm = JWT_ALG_RS384;
	else if (algorithm == "RS512")
		m_algorithm = JWT_ALG_RS512;
	else if (algorithm == "NONE")
		m_algorithm = JWT_ALG_NONE;
	else
		throw InvalidArgumentException("unknown algorithm " + algorithm);
}

static Array::Ptr serializeArray(const vector<string> &array)
{
	Array::Ptr arr = new Array;

	for (auto i : array)
		arr->add(i);

	return arr;
}

static void insertClaims(jwt_t *tokenRaw, const JWToken &token)
{
	int err;

	err = ::jwt_del_grants(tokenRaw, NULL);
	if (err != 0)
		throw IllegalStateException(Error::getMessage(err));

	if (!token.issuer().empty()) {
		err = ::jwt_add_grant(tokenRaw, "iss", token.issuer().c_str());
		if (err != 0)
			throw IllegalStateException("iss: " + Error::getMessage(err));
	}

	if (!token.subject().empty()) {
		err = ::jwt_add_grant(tokenRaw, "sub", token.subject().c_str());
		if (err != 0)
			throw IllegalStateException("sub: " + Error::getMessage(err));
	}

	if (!token.audience().empty()) {
		Object::Ptr object = new Object;
		object->set("aud", serializeArray(token.audience()));
		const string &aud = Dynamic::Var::toString(object);

		err = ::jwt_add_grants_json(tokenRaw, aud.c_str());
		if (err != 0)
			throw IllegalStateException("aud: " + Error::getMessage(err));
	}

	if (!token.locale().empty()) {
		err = ::jwt_add_grant(tokenRaw, "locale", token.locale().c_str());
		if (err != 0)
			throw IllegalStateException("locale: " + Error::getMessage(err));
	}

	if (!token.expirationTime().isNull()) {
		const long exp = token.expirationTime().value().epochTime();

		err = ::jwt_add_grant_int(tokenRaw, "exp", exp);
		if (err != 0)
			throw IllegalStateException("exp: " + Error::getMessage(err));
	}

	if (!token.issuedAt().isNull()) {
		const long iat = token.issuedAt().value().epochTime();

		err = ::jwt_add_grant_int(tokenRaw, "iat", iat);
		if (err != 0)
			throw IllegalStateException("iat: " + Error::getMessage(err));
	}

	if (!token.notBefore().isNull()) {
		const long nbf = token.notBefore().value().epochTime();

		err = ::jwt_add_grant_int(tokenRaw, "nbf", nbf);
		if (err != 0)
			throw IllegalStateException("nbf: " + Error::getMessage(err));
	}
}

TokenID JWTEncoder::encode(const JWToken &token) const
{
	string tokenId;
	char *tokenIdRaw;

	const unsigned char *secret;
	if (m_secret.empty())
		secret = NULL;
	else
		secret = reinterpret_cast<const unsigned char *>(m_secret.c_str());

	jwt_t *tokenRaw;
	int err = ::jwt_new(&tokenRaw);
	if (err != 0)
		throw IllegalStateException("jwt alloc: " + Error::getMessage(err));

	try {
		err = ::jwt_set_alg(tokenRaw, m_algorithm, secret, m_secret.length());
		if (err != 0)
			throw InvalidArgumentException("algorithm: " + Error::getMessage(err));

		insertClaims(tokenRaw, token);

	} catch (...) {
		::jwt_free(tokenRaw);
		throw;
	}

	tokenIdRaw = ::jwt_encode_str(tokenRaw);
	::jwt_free(tokenRaw);

	if (tokenIdRaw == NULL)
		throw IllegalStateException("jwt encode: " + Error::getMessage(Error::last()));

	try {
		// we are paranoid to accidently not leak resources
		if (logger().debug())
			logger().debug("encoded as " + string(tokenIdRaw), __FILE__, __LINE__);

		tokenId = string(tokenIdRaw);

	} catch (...) {
		::free(tokenIdRaw);
		throw;
	}

	::free(tokenIdRaw);

	return TokenID::parse(tokenId);
}
