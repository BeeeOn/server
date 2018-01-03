#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/Timespan.h>

#include "l10n/LocaleManager.h"
#include "l10n/SystemLocaleManager.h"
#include "server/JWTSessionManager.h"
#include "jwt/JWTDecoder.h"
#include "jwt/JWTEncoder.h"
#include "cppunit/BetterAssert.h"

using namespace std;

namespace BeeeOn {

class JWTSessionManagerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(JWTSessionManagerTest);
	CPPUNIT_TEST(testOpenClose);
	CPPUNIT_TEST(testNotInAudience);
	CPPUNIT_TEST(testMissingLocale);
	CPPUNIT_TEST(testInvalidLocale);
	CPPUNIT_TEST(testSessionTimeout);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testOpenClose();
	void testNotInAudience();
	void testMissingLocale();
	void testInvalidLocale();
	void testSessionTimeout();

private:
	JWTSessionManager m_manager;
	LocaleManager::Ptr m_localeManager;
	JWTDecoder::Ptr m_decoder;
	JWTEncoder::Ptr m_encoder;
};

CPPUNIT_TEST_SUITE_REGISTRATION(JWTSessionManagerTest);

void JWTSessionManagerTest::setUp()
{
	m_localeManager = new SystemLocaleManager;
	m_decoder = new JWTDecoder;
	m_encoder = new JWTEncoder;

	const string &secret = "SuperClever_unGuEsSabLE-s:E#c~R.e,T";
	m_decoder->setSecret(secret);
	m_encoder->setSecret(secret);
	m_encoder->setAlgorithm("HS256");

	m_manager.setSessionExpireTime(2 * Poco::Timespan::SECONDS);
	m_manager.setLocaleManager(m_localeManager);
	m_manager.setJWTDecoder(m_decoder);
	m_manager.setJWTEncoder(m_encoder);
	m_manager.setAudienceList({"BeeeOn"});
	m_manager.setIssuerName("BeeeOn");
}

void JWTSessionManagerTest::tearDown()
{
}

void JWTSessionManagerTest::testOpenClose()
{
	UserID userID = UserID::parse("824b4831-6ce4-4614-8e02-8380d6d92f95");
	VerifiedIdentityID identityID = VerifiedIdentityID::parse("be572079-0971-47f0-95df-68476a642911");

	VerifiedIdentity identity(identityID);
	User user(userID);
	identity.setUser(user);

	Session::Ptr newSession;
	CPPUNIT_ASSERT_NO_THROW(newSession = m_manager.open(identity));
	const SessionID &id = newSession->sessionID();

	Session::Ptr infoLookup;
	CPPUNIT_ASSERT(m_manager.lookup(id, infoLookup));

	CPPUNIT_ASSERT(infoLookup->identityID() == identityID);
	CPPUNIT_ASSERT(infoLookup->userID() == userID);
	CPPUNIT_ASSERT(infoLookup->getExpiration().elapsed() <= 2 * Poco::Timespan::SECONDS);
}

/**
 * Token:
 * {
 *   "typ": "JWT",
 *   "alg": "HS256"
 * }
 * {
 *   "aud": [
 *     "UNKNOWN"
 *   ],
 *   "exp": 9223372036854,
 *   "iat": -9223372036854,
 *   "iss": "BeeeOn",
 *   "nbf": -9223372036854,
 *   "sub": "vlcgeQlxR/CV32hHamQpEYJLSDFs5EYUjgKDgNbZL5U="
 * }
 */
static const string TOKEN_WITH_UNKNOWN_IN_AUDIENCE =
	"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOlsiVU5LTk9XTiJdLCJleH"
	"AiOjkyMjMzNzIwMzY4NTQsImlhdCI6LTkyMjMzNzIwMzY4NTQsImlzcyI6IkJlZWVPb"
	"iIsImxvY2FsZSI6ImtvY3Vya292byIsIm5iZiI6LTkyMjMzNzIwMzY4NTQsInN1YiI6"
	"InZsY2dlUWx4Ui9DVjMyaEhhbVFwRVlKTFNERnM1RVlVamdLRGdOYlpMNVU9In0.W1B"
	"RGHTRow6TtrmDm-Bzj9CwEh0vNKolJFc62KObha4";

/**
 * Tests token that does not have tested issuer name in audience.
 */
void JWTSessionManagerTest::testNotInAudience()
{
	SessionID id(TOKEN_WITH_UNKNOWN_IN_AUDIENCE);
	Session::Ptr infoLookup;

	CPPUNIT_ASSERT(!m_manager.lookup(id, infoLookup));
}

/**
 * Token:
 * {
 *   "typ": "JWT",
 *   "alg": "HS256"
 * }
 * {
 *   "aud": [
 *     "BeeeOn"
 *   ],
 *   "exp": 9223372036854,
 *   "iat": -9223372036854,
 *   "iss": "BeeeOn",
 *   "nbf": -9223372036854,
 *   "sub": "vlcgeQlxR/CV32hHamQpEYJLSDFs5EYUjgKDgNbZL5U="
 * }
 */
static const string TOKEN_WITH_NO_LOCALE_CLAIM =
	"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOlsiQmVlZU9uIl0sImV4cCI6OTI"
	"yMzM3MjAzNjg1NCwiaWF0IjotOTIyMzM3MjAzNjg1NCwiaXNzIjoiQmVlZU9uIiwibmJmIjo"
	"tOTIyMzM3MjAzNjg1NCwic3ViIjoidmxjZ2VRbHhSL0NWMzJoSGFtUXBFWUpMU0RGczVFWVV"
	"qZ0tEZ05iWkw1VT0ifQ.1hMi5-fIGPJnGtwE_O9M7W0faNKppsQsmNyEMmO_y9k";

/**
 * Test of manager processing token with no locale claim.
 */
void JWTSessionManagerTest::testMissingLocale()
{
	SessionID id(TOKEN_WITH_NO_LOCALE_CLAIM);
	Session::Ptr infoLookup;

	CPPUNIT_ASSERT_NO_THROW(m_manager.lookup(id, infoLookup));
	CPPUNIT_ASSERT_EQUAL(infoLookup->locale().toString(), Locale().toString());
}

/**
 * Token:
 * {
 *   "typ": "JWT",
 *   "alg": "HS256"
 * }
 * {
 *   "aud": [
 *     "BeeeOn"
 *   ],
 *   "exp": 9223372036854,
 *   "iat": -9223372036854,
 *   "iss": "BeeeOn",
 *   "locale": "kocurkovo",
 *   "nbf": -9223372036854,
 *   "sub": "vlcgeQlxR/CV32hHamQpEYJLSDFs5EYUjgKDgNbZL5U="
 * }
 */
static const string TOKEN_WITH_INVALID_LOCALE_CLAIM =
	"eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJhdWQiOlsiQmVlZU9uIl0sImV4cCI"
	"6OTIyMzM3MjAzNjg1NCwiaWF0IjotOTIyMzM3MjAzNjg1NCwiaXNzIjoiQmVlZU9uIiw"
	"ibG9jYWxlIjoia29jdXJrb3ZvIiwibmJmIjotOTIyMzM3MjAzNjg1NCwic3ViIjoidmx"
	"jZ2VRbHhSL0NWMzJoSGFtUXBFWUpMU0RGczVFWVVqZ0tEZ05iWkw1VT0ifQ.a97ry6lx"
	"9GsgW1G2a_g36ihUAfeUjOFKxYofsBVzT5M";

/**
 * Test of manager processing token that has invalid value of locale.
 */
void JWTSessionManagerTest::testInvalidLocale()
{
	SessionID id(TOKEN_WITH_INVALID_LOCALE_CLAIM);
	Session::Ptr infoLookup;

	CPPUNIT_ASSERT_NO_THROW(m_manager.lookup(id, infoLookup));
	CPPUNIT_ASSERT_EQUAL(infoLookup->locale().toString(), Locale().toString());
}

/**
 * Test of manager responding to expired token.
 */
void JWTSessionManagerTest::testSessionTimeout()
{
	m_manager.setSessionExpireTime(0);

	Session::Ptr infoLookup;
	UserID userID = UserID::parse("824b4831-6ce4-4614-8e02-8380d6d92f95");
	VerifiedIdentityID identityID = VerifiedIdentityID::parse("be572079-0971-47f0-95df-68476a642911");

	VerifiedIdentity identity(identityID);
	User user(userID);
	identity.setUser(user);

	Session::Ptr session = m_manager.open(identity);

	CPPUNIT_ASSERT(!m_manager.lookup(session->sessionID(), infoLookup));
}

}
