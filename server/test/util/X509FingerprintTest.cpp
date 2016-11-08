#include <sstream>
#include <Poco/DateTime.h>
#include <Poco/DateTimeFormat.h>
#include <Poco/DateTimeFormatter.h>
#include <Poco/Crypto/X509Certificate.h>
#include <Poco/Net/Utility.h>

#include <cppunit/extensions/HelperMacros.h>

#include "util/X509Fingerprint.h"

using namespace std;
using namespace Poco;
using namespace Poco::Crypto;

namespace BeeeOn {

class X509FingerprintTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(X509FingerprintTest);
	CPPUNIT_TEST(testCertInfo);
	CPPUNIT_TEST(testMD5);
	CPPUNIT_TEST(testSHA1);
	CPPUNIT_TEST(testSHA256);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testCertInfo();
	void testMD5();
	void testSHA1();
	void testSHA256();

private:
	X509Certificate *m_cert;
};

CPPUNIT_TEST_SUITE_REGISTRATION(X509FingerprintTest);

#define TEST_CERT std::string( \
"-----BEGIN CERTIFICATE-----\n" \
"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\n" \
"AW9e5n+bKBu9Ul0y3YyZ5T0W\n" \
"-----END CERTIFICATE-----")

#define TEST_CERT_NAME std::string( \
		"/C=CZ/ST=Czech Republic/L=Brno/O=IoT/" \
		"emailAddress=ca@iot.example.com/" \
		"CN=iot.example.com")

#define TEST_CERT_COMMON_NAME std::string("iot.example.com")

#define TEST_CERT_VALID_FROM std::string( \
		"2016-11-08 14:31:08")

#define TEST_CERT_MD5    std::string( \
		"066bea99b0614342f632db843825a607")
#define TEST_CERT_SHA1   std::string( \
		"5b2659a0dc9294ac079d7c7cee56f76c" \
		"6dd21460")
#define TEST_CERT_SHA256 std::string( \
		"ed78b4e614d630649a849c5da19d72c0" \
		"2d10b37cdf90b57cd7a0638fc71fa280")

void X509FingerprintTest::setUp()
{
	initializeCrypto();
	istringstream cert(TEST_CERT);

	m_cert = new X509Certificate(cert);
}

void X509FingerprintTest::tearDown()
{
	delete m_cert;
	uninitializeCrypto();
}

void X509FingerprintTest::testCertInfo()
{
	const string &validFrom =
		DateTimeFormatter::format(m_cert->validFrom(),
				DateTimeFormat::SORTABLE_FORMAT);

	CPPUNIT_ASSERT_EQUAL(TEST_CERT_COMMON_NAME, m_cert->commonName());
	CPPUNIT_ASSERT_EQUAL(TEST_CERT_NAME, m_cert->subjectName());
	CPPUNIT_ASSERT_EQUAL(TEST_CERT_NAME, m_cert->issuerName());
	CPPUNIT_ASSERT_EQUAL(TEST_CERT_VALID_FROM, validFrom);
}

void X509FingerprintTest::testMD5()
{
	try {
	X509Fingerprint fingerprint(m_cert);
	const string &md5 = fingerprint.digestToHex("MD5");

	CPPUNIT_ASSERT_EQUAL(TEST_CERT_MD5, md5);
	} catch(const Exception &e) {
		std::cout << e.displayText() << std::endl;
		e.rethrow();
	}
}

void X509FingerprintTest::testSHA1()
{
	X509Fingerprint fingerprint(m_cert);
	const string &sha1 = fingerprint.digestToHex("SHA1");

	CPPUNIT_ASSERT_EQUAL(TEST_CERT_SHA1, sha1);
}

void X509FingerprintTest::testSHA256()
{
	X509Fingerprint fingerprint(m_cert);
	const string &sha256 = fingerprint.digestToHex("SHA256");

	CPPUNIT_ASSERT_EQUAL(TEST_CERT_SHA256, sha256);
}

}
