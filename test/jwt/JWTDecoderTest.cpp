#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/Timestamp.h>

#include "cppunit/BetterAssert.h"
#include "jwt/JWToken.h"
#include "jwt/JWTConstants.h"
#include "jwt/JWTDecoder.h"
#include "model/TokenID.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class JWTDecoderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(JWTDecoderTest);
	CPPUNIT_TEST(testDecodeWithHS256);
	CPPUNIT_TEST(testDecodeWithHS384);
	CPPUNIT_TEST(testDecodeWithHS512);
	CPPUNIT_TEST(testDecodeWithRS256);
	CPPUNIT_TEST(testDecodeWithRS384);
	CPPUNIT_TEST(testDecodeWithRS512);
	CPPUNIT_TEST(testDecodeWithNONE);
	CPPUNIT_TEST(testNonExistingAlg);
	CPPUNIT_TEST(testInvalidHeader);
	CPPUNIT_TEST(testInvalidSignature);
	CPPUNIT_TEST(testInvalidPayload);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void doTestDecode(const TokenID &input, const JWToken &expect);

	void testDecodeWithHS256();
	void testDecodeWithHS384();
	void testDecodeWithHS512();
	void testDecodeWithRS256();
	void testDecodeWithRS384();
	void testDecodeWithRS512();
	void testDecodeWithNONE();
	void testNonExistingAlg();
	void testInvalidHeader();
	void testInvalidSignature();
	void testInvalidPayload();

private:
	JWTDecoder m_decoder;
	JWToken m_token;
};

CPPUNIT_TEST_SUITE_REGISTRATION(JWTDecoderTest);

void JWTDecoderTest::setUp()
{
	m_decoder.setSecret(""); // clear

	m_token.setIssuer("BeeeOn");
	m_token.setSubject("be572079-0971-47f0-95df-68476a642911");
	m_token.setLocale("en_US");
	m_token.setAudience({"BeeeOn"});
	m_token.setExpirationTime(Timestamp::TIMEVAL_MAX);
	m_token.setIssuedAt(Timestamp::TIMEVAL_MIN);
	m_token.setNotBefore(Timestamp::TIMEVAL_MIN);
}

void JWTDecoderTest::doTestDecode(const TokenID &input, const JWToken &expect)
{
	JWToken result;
	
	CPPUNIT_ASSERT_NO_THROW(result = m_decoder.decode(input));

	CPPUNIT_ASSERT_EQUAL(expect.issuer(), result.issuer());
	CPPUNIT_ASSERT_EQUAL(expect.subject(), result.subject());
	CPPUNIT_ASSERT_EQUAL(expect.locale(), result.locale());

	CPPUNIT_ASSERT_EQUAL(expect.expirationTime().value().epochTime(),
			     result.expirationTime().value().epochTime());
	CPPUNIT_ASSERT_EQUAL(expect.issuedAt().value().epochTime(),
			     result.issuedAt().value().epochTime());
	CPPUNIT_ASSERT_EQUAL(expect.notBefore().value().epochTime(),
		             result.notBefore().value().epochTime());

	CPPUNIT_ASSERT(expect.audience() == result.audience());
}

void JWTDecoderTest::testDecodeWithHS256()
{
	m_decoder.setSecret(JWTConstants::HMAC_SECRET);
	doTestDecode(JWTConstants::HS256_VALID_TOKEN, m_token);
}

void JWTDecoderTest::testDecodeWithHS384()
{
	m_decoder.setSecret(JWTConstants::HMAC_SECRET);
	doTestDecode(JWTConstants::HS384_VALID_TOKEN, m_token);
}

void JWTDecoderTest::testDecodeWithHS512()
{
	m_decoder.setSecret(JWTConstants::HMAC_SECRET);
	doTestDecode(JWTConstants::HS512_VALID_TOKEN, m_token);
}

void JWTDecoderTest::testDecodeWithRS256()
{
	m_decoder.setSecret(JWTConstants::RSA_SECRET);
	doTestDecode(JWTConstants::RS256_VALID_TOKEN, m_token);
}

void JWTDecoderTest::testDecodeWithRS384()
{
	m_decoder.setSecret(JWTConstants::RSA_SECRET);
	doTestDecode(JWTConstants::RS384_VALID_TOKEN, m_token);
}

void JWTDecoderTest::testDecodeWithRS512()
{
	m_decoder.setSecret(JWTConstants::RSA_SECRET);
	doTestDecode(JWTConstants::RS512_VALID_TOKEN, m_token);
}

void JWTDecoderTest::testDecodeWithNONE()
{
	m_decoder.setSecret("");
	doTestDecode(JWTConstants::NONAL_VALID_TOKEN, m_token);
}

/**
 * Trying to encode and decode token with non existing algorithm.
 *
 * To decode token, Base64 encoded token is provided:
 * - Header:  {"alg": "UNICORNL_ALG","typ": "HS256"}
 * - Payload: {"sub": "1234567890", "name": "John Doe", "admin": true}
 */
void JWTDecoderTest::testNonExistingAlg()
{
	m_decoder.setSecret("");

	const string token =
		"eyJhbGciOiJVTklDT1JOX0FMRyIsInR5cCI6Imp3dCJ9.eyJzdWIiOiIxMjM0NTY3ODk"
		"wIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9.";

	CPPUNIT_ASSERT_THROW(m_decoder.decode(token), InvalidArgumentException);
}

/**
 * Trying to decode token with invalid header:
 *
 * {"alg": "HS256", "typ": "DEVIL_TOKEN"}
 */
void JWTDecoderTest::testInvalidHeader()
{
	const string token =
		"eyJhbGciOiJIUzI1NiIsInR5cCI6IkRFVklMX1RPS0VOIn0.eyJzdWIiOiIxMjM0NTY"
		"3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9.";

	CPPUNIT_ASSERT_THROW(m_decoder.decode(token), InvalidArgumentException);
}

void JWTDecoderTest::testInvalidSignature()
{
	m_decoder.setSecret(JWTConstants::HMAC_SECRET);
	const string invalidToken = JWTConstants::HS256_VALID_TOKEN + "corrupted signature";

	CPPUNIT_ASSERT_THROW(m_decoder.decode(invalidToken), InvalidArgumentException);
}

/**
 * Testing decode token with invalid "exp" and "iat" in the payload:
 *
 * {
 *  "aud": "Everyone",
 *  "exp": "day before tomorrow",
 *  "iat": "maybe today?",
 *  "iss": ["Harry Potter"],
 *  "locale": 42,
 *  "nbf": 13,
 *  "sub": 12
 * }
 */
void JWTDecoderTest::testInvalidPayload()
{
	m_decoder.setSecret(JWTConstants::HMAC_SECRET);

	const string invalidToken =
		"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOiJFdmVyeW9uZSIsImV4cCI6Im"
		"RheSBiZWZvcmUgdG9tb3Jyb3ciLCJpYXQiOiJtYXliZSB0b2RheT8iLCJpc3MiOlsiSGFyc"
		"nkgUG90dGVyIl0sImxvY2FsZSI6NDIsIm5iZiI6MTMsInN1YiI6MTJ9.hU6QnKL9Hw2O0Q_"
		"4j_HK2KMck079mEdy5v3TcnSf2QI";

	CPPUNIT_ASSERT_THROW(m_decoder.decode(invalidToken), SyntaxException);
}

}
