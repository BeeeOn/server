#include <Poco/Exception.h>
#include <Poco/Environment.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/HTMLForm.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/SkippableAutoRegisterSuite.h"
#include "provider/AuthProvider.h"
#include "provider/FacebookAuthProvider.h"
#include "ssl/SSLClient.h"

using namespace Poco;
using namespace Poco::Net;
using namespace std;

namespace BeeeOn {

bool skipWhenNotSetup()
{
	if (!Environment::has("FACEBOOK_AUTH_CODE"))
		return true;
	if (!Environment::has("FACEBOOK_CLIENT_SECRET"))
		return true;
	if (!Environment::has("FACEBOOK_CLIENT_ID"))
		return true;
	return false;
}

class FacebookAuthProviderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FacebookAuthProviderTest);
	CPPUNIT_TEST(testVerifyAuthCode);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void testVerifyAuthCode();
private:
	string m_authCode;
	SharedPtr<SSLClient> m_sslConfig;
};

CPPUNIT_TEST_SUITE_REGISTRATION_SKIPPABLE(FacebookAuthProviderTest,
		skipWhenNotSetup);

class TestableFacebookAuthProvider : public FacebookAuthProvider {
public:
	using FacebookAuthProvider::verifyAuthCode;
};

void FacebookAuthProviderTest::setUp()
{
	//Obtain auth code by accessing:
	//https://www.facebook.com/v2.9/dialog/oauth?client_id={CLIENT_ID}&redirect_uri={REDIRECT_URI}&response_type=code
	m_authCode = Environment::get("FACEBOOK_AUTH_CODE");
}

void FacebookAuthProviderTest::testVerifyAuthCode()
{
	TestableFacebookAuthProvider provider;
	AuthResult info;
	SharedPtr<SSLClient> sslConfig = new SSLClient;

	if (Environment::has("FACEBOOK_CA_LOCATION"))
		sslConfig->setCALocation(Environment::get("FACEBOOK_CA_LOCATION"));
	else
		sslConfig->setCALocation("cert/mozilla-cacert-2016-11-02.pem");

	provider.setSSLConfig(sslConfig);
	provider.setClientId(Environment::get("FACEBOOK_CLIENT_ID"));
	provider.setClientSecret(Environment::get("FACEBOOK_CLIENT_SECRET"));

	AuthCodeCredentials credentials("facebook", m_authCode);

	//MUST be same as redirect uri that was used to get auth code/acces token
	provider.setRedirectURI(
			Environment::get("FACEBOOK_REDIRECT_URI", "https://localhost/"));
	CPPUNIT_ASSERT_MESSAGE("failed to authenticate user",
			provider.verifyAuthCode(credentials, info));
	CPPUNIT_ASSERT_MESSAGE("id field has not been obtained",
			!info.providerID().empty());
	CPPUNIT_ASSERT_MESSAGE("email field has not been obtained",
			!info.email().empty());
	}
}


