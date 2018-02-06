#include <sstream>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/SharedPtr.h>
#include <Poco/Net/X509Certificate.h>
#include <Poco/Net/SSLException.h>

#include "cppunit/BetterAssert.h"
#include "gws/X509GatewayPeerVerifier.h"
#include "ssl/X509Fingerprint.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

namespace BeeeOn {

class X509GatewayPeerVerifierTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(X509GatewayPeerVerifierTest);
	CPPUNIT_TEST(testValidCert);
	CPPUNIT_TEST(testVerifyPeer);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();

	void testValidCert();
	void testVerifyPeer();

private:
	SharedPtr<X509Certificate> m_cert1806037847304877;
};

CPPUNIT_TEST_SUITE_REGISTRATION(X509GatewayPeerVerifierTest);

void X509GatewayPeerVerifierTest::setUp()
{
	istringstream cert;
	cert.str( // gateway: 1806037847304877
	"-----BEGIN CERTIFICATE-----\n"
	"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\n"
	"S1eUSMZ86VrxFXnkXKSJkCwOB3sjikv9KaU=\n"
	"-----END CERTIFICATE-----\n");

	m_cert1806037847304877 = new X509Certificate(cert);
}

/**
 * Check contents of the certificate.
 */
void X509GatewayPeerVerifierTest::testValidCert()
{
	CPPUNIT_ASSERT_EQUAL("1806037847304877",
			m_cert1806037847304877->commonName());

	CPPUNIT_ASSERT_EQUAL(
			"/CN=1806037847304877/C=CZ/ST=Czech Republic"
			"/L=Brno/O=BeeeOn/emailAddress=testing@beeeon.org",
			m_cert1806037847304877->subjectName());

	CPPUNIT_ASSERT_EQUAL(
			"/CN=1806037847304877/C=CZ/ST=Czech Republic"
			"/L=Brno/O=BeeeOn/emailAddress=testing@beeeon.org",
			m_cert1806037847304877->issuerName());

	CPPUNIT_ASSERT_EQUAL("c1bc9c64ac069c93c7e219365247e3fe59a0396f8aa8862c0075bbc4b804e44e",
			X509Fingerprint(m_cert1806037847304877).digestToHex("SHA256"));
}

void X509GatewayPeerVerifierTest::testVerifyPeer()
{
	X509GatewayPeerVerifier verifier(*m_cert1806037847304877);
	const Gateway matching(GatewayID::parse("1806037847304877"));
	const Gateway other(GatewayID::parse("1467642221773030"));

	CPPUNIT_ASSERT_NO_THROW(verifier.verifyPeer(matching));

	CPPUNIT_ASSERT_THROW(verifier.verifyPeer(other),
			CertificateValidationException);
}

}
