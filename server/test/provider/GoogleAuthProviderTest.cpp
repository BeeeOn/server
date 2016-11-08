#include <Poco/Exception.h>
#include <Poco/Environment.h>

#include <cppunit/extensions/HelperMacros.h>

#include "provider/GoogleAuthProvider.h"
#include "provider/AuthProvider.h"
#include "util/SSLClient.h"
#include "cppunit/SkippableAutoRegisterSuite.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

bool skipWhenNoAuthCode()
{
	if (!Environment::has("GOOGLE_AUTH_CODE"))
		return true;
	if (!Environment::has("GOOGLE_CLIENT_ID"))
		return true;
	if (!Environment::has("GOOGLE_CLIENT_SECRET"))
		return true;

	return false;
}

class GoogleAuthProviderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(GoogleAuthProviderTest);
	CPPUNIT_TEST(testVerifyAuthCode);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testVerifyAuthCode();
private:
	std::string googleAuthCode;
	SSLClient *m_sslConfig;
};

CPPUNIT_TEST_SUITE_REGISTRATION_SKIPPABLE(GoogleAuthProviderTest,
		skipWhenNoAuthCode);

class TestableGoogleAuthProvider : public GoogleAuthProvider {
public:
	using GoogleAuthProvider::verifyAuthCode;
};

void GoogleAuthProviderTest::setUp()
{
	googleAuthCode = Environment::get("GOOGLE_AUTH_CODE");
}

void GoogleAuthProviderTest::tearDown()
{
}

/**
 * Verify proper Google user info fetching from Google API.
 * The test requires environmental variable GOOGLE_AUTH_CODE to be set.
 * It is an authorization_code from googleapis.com obtained from webapp.
 *
 * TODO: obtain the authorization_code automatically (if it is even possible).
 */
void GoogleAuthProviderTest::testVerifyAuthCode()
{

	TestableGoogleAuthProvider provider;
	AuthResult info;
	SSLClient sslConfig;

	if (Environment::has("GOOGLE_CA_LOCATION"))
		sslConfig.setCALocation(Environment::get("GOOGLE_CA_LOCATION"));
	else
		sslConfig.setCALocation("cert/mozilla-cacert-2016-11-02.pem");

	provider.setSSLConfig(&sslConfig);
	provider.setClientId(Environment::get("GOOGLE_CLIENT_ID"));
	provider.setClientSecret(Environment::get("GOOGLE_CLIENT_SECRET"));

	if (Environment::has("GOOGLE_REDIRECT_URI"))
		provider.setRedirectURI(Environment::get("GOOGLE_REDIRECT_URI"));
	else
		provider.setRedirectURI("http://localhost");

	CPPUNIT_ASSERT_MESSAGE("failed to authenticate user",
			provider.verifyAuthCode(googleAuthCode, info));

	CPPUNIT_ASSERT_MESSAGE("missing email field",
			!info.email().empty());
	CPPUNIT_ASSERT_MESSAGE("missing google_id field",
			!info.providerID().empty());
}
}


