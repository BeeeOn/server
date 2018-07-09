#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/Timestamp.h>

#include "cppunit/BetterAssert.h"
#include "jwt/JWToken.h"
#include "jwt/JWTConstants.h"
#include "jwt/JWTEncoder.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class JWTEncoderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(JWTEncoderTest);
	CPPUNIT_TEST(testEncodeWithHS256);
	CPPUNIT_TEST(testEncodeWithHS384);
	CPPUNIT_TEST(testEncodeWithHS512);
	CPPUNIT_TEST(testEncodeWithRS256);
	CPPUNIT_TEST(testEncodeWithRS384);
	CPPUNIT_TEST(testEncodeWithRS512);
	CPPUNIT_TEST(testEncodeWithNONE);
	CPPUNIT_TEST(testNonExistingAlg);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void doTestEncode(const JWToken &input, const TokenID &expect);

	void testEncodeWithHS256();
	void testEncodeWithHS384();
	void testEncodeWithHS512();
	void testEncodeWithRS256();
	void testEncodeWithRS384();
	void testEncodeWithRS512();
	void testEncodeWithNONE();
	void testNonExistingAlg();

private:
	JWTEncoder m_encoder;
	JWToken m_token;
};

CPPUNIT_TEST_SUITE_REGISTRATION(JWTEncoderTest);

void JWTEncoderTest::setUp()
{
	m_encoder.setSecret(""); // clear
	m_encoder.setAlgorithm("NONE");

	m_token.setIssuer("BeeeOn");
	m_token.setSubject("be572079-0971-47f0-95df-68476a642911");
	m_token.setLocale("en_US");
	m_token.setAudience({"BeeeOn"});
	m_token.setExpirationTime(Timestamp::TIMEVAL_MAX);
	m_token.setIssuedAt(Timestamp::TIMEVAL_MIN);
	m_token.setNotBefore(Timestamp::TIMEVAL_MIN);
}

void JWTEncoderTest::doTestEncode(const JWToken &input, const TokenID &expect)
{
	TokenID result;

	CPPUNIT_ASSERT_NO_THROW(result = m_encoder.encode(input));
	CPPUNIT_ASSERT_EQUAL(expect.toString(), result.toString());
}

void JWTEncoderTest::testEncodeWithHS256()
{
	m_encoder.setAlgorithm("HS256");
	m_encoder.setSecret(JWTConstants::HMAC_SECRET);

	doTestEncode(m_token, JWTConstants::HS256_VALID_TOKEN);
}

void JWTEncoderTest::testEncodeWithHS384()
{
	m_encoder.setAlgorithm("HS384");
	m_encoder.setSecret(JWTConstants::HMAC_SECRET);

	doTestEncode(m_token, JWTConstants::HS384_VALID_TOKEN);
}

void JWTEncoderTest::testEncodeWithHS512()
{
	m_encoder.setAlgorithm("HS512");
	m_encoder.setSecret(JWTConstants::HMAC_SECRET);

	doTestEncode(m_token, JWTConstants::HS512_VALID_TOKEN);
}

void JWTEncoderTest::testEncodeWithRS256()
{
	m_encoder.setAlgorithm("RS256");
	m_encoder.setSecret(JWTConstants::RSA_SECRET);

	doTestEncode(m_token, JWTConstants::RS256_VALID_TOKEN);
}

void JWTEncoderTest::testEncodeWithRS384()
{
	m_encoder.setAlgorithm("RS384");
	m_encoder.setSecret(JWTConstants::RSA_SECRET);

	doTestEncode(m_token, JWTConstants::RS384_VALID_TOKEN);
}

void JWTEncoderTest::testEncodeWithRS512()
{
	m_encoder.setAlgorithm("RS512");
	m_encoder.setSecret(JWTConstants::RSA_SECRET);

	doTestEncode(m_token, JWTConstants::RS512_VALID_TOKEN);
}

void JWTEncoderTest::testEncodeWithNONE()
{
	m_encoder.setAlgorithm("NONE");
	m_encoder.setSecret("");

	doTestEncode(m_token, JWTConstants::NONAL_VALID_TOKEN);
}

/**
 * Trying to setup encoder with a non-existing algorithm.
 */
void JWTEncoderTest::testNonExistingAlg()
{
	m_encoder.setSecret("");
	CPPUNIT_ASSERT_THROW(m_encoder.setAlgorithm("UNICORN_ALG"), InvalidArgumentException);
}

}
