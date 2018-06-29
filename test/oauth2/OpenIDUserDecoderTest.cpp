#include <Poco/Exception.h>
#include <Poco/Net/NetException.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "oauth2/OpenIDUserDecoder.h"

using namespace Poco;
using namespace Poco::Net;
using namespace std;

namespace BeeeOn {

/**
 * <pre>
 * "sub": "3rd Party Service",
 * "aud": ["BeeeOn"],
 * "iss": "3rd Party",
 * "iat": 1530268102,
 * "nbf": 1530268112,
 * "exp": 1530268127,
 * "first_name": "John",
 * "last_name": "Doe",
 * "email": "john.doe@exmaple.org",
 * "locale": "en_US"
 * </pre>
 */
static const string ID_TOKEN =
	"eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9"
	".eyJzdWIiOiIzcmQgUGFydHkgU2VydmljZSIsImF1ZCI6"
	"WyJCZWVlT24iXSwiaXNzIjoiM3JkIFBhcnR5IiwiaWF0I"
	"joxNTMwMjY4MTAyLCJuYmYiOjE1MzAyNjgxMTIsImV4cC"
	"I6MTUzMDI2ODEyNywiZ2l2ZW5fbmFtZSI6IkpvaG4iLCJ"
	"mYW1pbHlfbmFtZSI6IkRvZSIsImVtYWlsIjoiam9obi5k"
	"b2VAZXhhbXBsZS5vcmciLCJsb2NhbGUiOiJlbl9VUyJ9"
	".QmDIMThJbLoLsdwc_oWES4FiTPPOx-0uJrneac0V8kI";

static const Timestamp TS_VALID = Timestamp::fromEpochTime(1530268118);
static const Timestamp TS_NBF_FAILS = Timestamp::fromEpochTime(1530268110);
static const Timestamp TS_EXP_FAILS = Timestamp::fromEpochTime(1530268128);

class OpenIDUserDecoderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(OpenIDUserDecoderTest);
	CPPUNIT_TEST(testDecodeIDToken);
	CPPUNIT_TEST(testMissingIDToken);
	CPPUNIT_TEST(testNonAcceptableIDToken);
	CPPUNIT_TEST(testExpiredIDToken);
	CPPUNIT_TEST(testClientIDBadIDToken);
	CPPUNIT_TEST(testInvalidIDToken);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void testDecodeIDToken();
	void testMissingIDToken();
	void testNonAcceptableIDToken();
	void testExpiredIDToken();
	void testClientIDBadIDToken();
	void testInvalidIDToken();

private:
	OpenIDUserDecoder m_decoder;
	JWTDecoder::Ptr m_jwtDecoder;
};

CPPUNIT_TEST_SUITE_REGISTRATION(OpenIDUserDecoderTest);

void OpenIDUserDecoderTest::setUp()
{
	m_jwtDecoder = new JWTDecoder;
	m_jwtDecoder->setSecret("my-testing-secret");

	m_decoder.setDecoder(m_jwtDecoder);
}

void OpenIDUserDecoderTest::testDecodeIDToken()
{
	m_decoder.setClientId("BeeeOn");
	OAuth2CodeExchanger::Tokens tokens;
	tokens.idToken = ID_TOKEN;
	AuthResult result;

	CPPUNIT_ASSERT_NO_THROW(m_decoder.fetch(tokens, result, TS_VALID));

	CPPUNIT_ASSERT_EQUAL("3rd Party Service", result.providerID());
	CPPUNIT_ASSERT_EQUAL("John", result.firstName());
	CPPUNIT_ASSERT_EQUAL("Doe", result.lastName());
	CPPUNIT_ASSERT_EQUAL("john.doe@example.org", result.email());
	CPPUNIT_ASSERT_EQUAL("en_US", result.locale());
}

void OpenIDUserDecoderTest::testMissingIDToken()
{
	m_decoder.setClientId("BeeeOn");
	OAuth2CodeExchanger::Tokens tokens;
	AuthResult result;

	CPPUNIT_ASSERT_THROW(
		m_decoder.fetch(tokens, result),
		NotAuthenticatedException);
}

void OpenIDUserDecoderTest::testNonAcceptableIDToken()
{
	m_decoder.setClientId("BeeeOn");
	OAuth2CodeExchanger::Tokens tokens;
	tokens.idToken = ID_TOKEN;
	AuthResult result;

	CPPUNIT_ASSERT_THROW(
		m_decoder.fetch(tokens, result, TS_NBF_FAILS),
		NotAuthenticatedException);
}

void OpenIDUserDecoderTest::testExpiredIDToken()
{
	m_decoder.setClientId("BeeeOn");
	OAuth2CodeExchanger::Tokens tokens;
	tokens.idToken = ID_TOKEN;
	AuthResult result;

	CPPUNIT_ASSERT_THROW(
		m_decoder.fetch(tokens, result, TS_EXP_FAILS),
		NotAuthenticatedException);
}

void OpenIDUserDecoderTest::testClientIDBadIDToken()
{
	m_decoder.setClientId("Something-else-then-BeeeOn");
	OAuth2CodeExchanger::Tokens tokens;
	tokens.idToken = ID_TOKEN;
	AuthResult result;

	CPPUNIT_ASSERT_THROW(
		m_decoder.fetch(tokens, result),
		NotAuthenticatedException);
}

void OpenIDUserDecoderTest::testInvalidIDToken()
{
	m_decoder.setClientId("BeeeOn");
	OAuth2CodeExchanger::Tokens tokens;
	tokens.idToken = "some input";
	AuthResult result;

	CPPUNIT_ASSERT_THROW(
		m_decoder.fetch(tokens, result),
		InvalidArgumentException);
}

}
