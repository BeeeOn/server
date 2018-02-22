#include <Poco/Exception.h>
#include <Poco/Environment.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/HTMLForm.h>

#include <cppunit/extensions/HelperMacros.h>

#include "provider/GoogleAuthProvider.h"
#include "provider/AuthProvider.h"
#include "ssl/SSLClient.h"
#include "cppunit/SkippableAutoRegisterSuite.h"

using namespace Poco::Net;
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
	CPPUNIT_TEST(testHTTPData);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testVerifyAuthCode();
	void testHTTPData();
private:
	std::string googleAuthCode;
	SharedPtr<SSLClient> m_sslConfig;
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

void GoogleAuthProviderTest::testHTTPData()
{
	HTMLForm form;
	form.setEncoding(HTMLForm::ENCODING_URL);
	form.set("code","any&redirect_uri=malicious&client_id=attacking_client_id");
	ostringstream code;
	form.write(code);
	string expected = "code=any%26redirect_uri%3Dmalicious%26client_id%3Dattacking_client_id";
	CPPUNIT_ASSERT_EQUAL(expected, code.str());
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
	SharedPtr<SSLClient> sslConfig = new SSLClient;

	if (Environment::has("GOOGLE_CA_LOCATION"))
		sslConfig->setCALocation(Environment::get("GOOGLE_CA_LOCATION"));
	else
		sslConfig->setCALocation("cert/mozilla-cacert-2018-01-17.pem");

	provider.setSSLConfig(sslConfig);
	provider.setClientId(Environment::get("GOOGLE_CLIENT_ID"));
	provider.setClientSecret(Environment::get("GOOGLE_CLIENT_SECRET"));

	if (Environment::has("GOOGLE_REDIRECT_URI"))
		provider.setRedirectURI(Environment::get("GOOGLE_REDIRECT_URI"));
	else
		provider.setRedirectURI("http://localhost");

	AuthCodeCredentials credentials("google", googleAuthCode);

	CPPUNIT_ASSERT_MESSAGE("failed to authenticate user",
			provider.verifyAuthCode(credentials, info));

	CPPUNIT_ASSERT_MESSAGE("missing email field",
			!info.email().empty());
	CPPUNIT_ASSERT_MESSAGE("missing google_id field",
			!info.providerID().empty());
}
}


