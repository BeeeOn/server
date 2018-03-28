#include <cstdlib>
#include <jwt.h>

#include <Poco/Error.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/JSONException.h>
#include <Poco/JSON/Parser.h>

#include "di/Injectable.h"
#include "jwt/JWTDecoder.h"
#include "model/TokenID.h"
#include "util/JsonUtil.h"

using namespace BeeeOn;
using namespace std;
using namespace Poco;
using namespace Poco::JSON;

BEEEON_OBJECT_BEGIN(BeeeOn, JWTDecoder)
BEEEON_OBJECT_PROPERTY("secret", &JWTDecoder::setSecret)
BEEEON_OBJECT_END(BeeeOn, JWTDecoder)

JWTDecoder::JWTDecoder()
{
}

JWTDecoder::~JWTDecoder()
{
}

static vector<string> deserializeArray(const string &arr)
{
	vector<string> deserialized;
	Parser parser;

	try {
		Array::Ptr array = parser.parse(arr).extract<Array::Ptr>();

		for (size_t i = 0; i < array->size(); i++)
			deserialized.push_back(array->getElement<string>(i));

		return deserialized;
	}
	catch (const JSONException &e) {
		throw SyntaxException("failed to parse array", e);
	}
}

static void fillTokenWithData(JWToken &token, jwt_t *tokenData)
{
	const char *claim = ::jwt_get_grant(tokenData, "iss");
	if (claim != NULL)
		token.setIssuer(string(claim));
	else
		token.setIssuer("");

	claim = ::jwt_get_grant(tokenData, "sub");
	if (claim != NULL)
		token.setSubject(string(claim));
	else
		token.setSubject("");

	char *jsonClaim = ::jwt_get_grants_json(tokenData, "aud");
	if (jsonClaim != NULL) {
		try {
			token.setAudience(deserializeArray(jsonClaim));
		} catch (...) {
			::free(jsonClaim);
			throw;
		}

		::free(jsonClaim);
	}

	claim = ::jwt_get_grant(tokenData, "locale");
	if (claim != NULL)
		token.setLocale(string(claim));
	else
		token.setLocale("");

	long timeRaw = ::jwt_get_grant_int(tokenData, "exp");
	if (Error::last() != ENOENT)
		token.setExpirationTime(Timestamp::fromEpochTime(timeRaw));
	else
		token.setExpirationTime(Nullable<Timestamp>());

	timeRaw = ::jwt_get_grant_int(tokenData, "iat");
	if (Error::last() != ENOENT)
		token.setIssuedAt(Timestamp::fromEpochTime(timeRaw));
	else
		token.setIssuedAt(Nullable<Timestamp>());

	timeRaw = ::jwt_get_grant_int(tokenData, "nbf");
	if (Error::last() != ENOENT)
		token.setNotBefore(Timestamp::fromEpochTime(timeRaw));
	else
		token.setNotBefore(Nullable<Timestamp>());
}

JWToken JWTDecoder::decode(const TokenID &tokenId) const
{
	JWToken token;
	const string &tokenID = tokenId.toString();

	const unsigned char *secret;

	if (m_secret.empty())
		secret = NULL;
	else
		secret = reinterpret_cast<const unsigned char *>(m_secret.c_str());

	const size_t secretLen = m_secret.length();

	jwt_t *tokenData;
	const int err = ::jwt_decode(&tokenData, tokenID.c_str(), secret, secretLen);
	if (err != 0)
		throw InvalidArgumentException("jwt decode: " + Error::getMessage(err));

	try {
		fillTokenWithData(token, tokenData);
	} catch (...) {
		::jwt_free(tokenData);
		throw;
	}

	::jwt_free(tokenData);

	return token;
}

void JWTDecoder::setSecret(const string &secret)
{
	m_secret = secret;
}
