#include <Poco/Exception.h>
#include <Poco/Environment.h>
#include <Poco/SharedPtr.h>
#include <Poco/Net/HTMLForm.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/SkippableAutoRegisterSuite.h"
#include "oauth2/FacebookOAuth2Provider.h"
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

class FacebookOAuth2ProviderTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(FacebookOAuth2ProviderTest);
	CPPUNIT_TEST(testVerifyAuthCode);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void testVerifyAuthCode();
private:
	string m_authCode;
	SSLClient::Ptr m_sslConfig;
};

CPPUNIT_TEST_SUITE_REGISTRATION_SKIPPABLE(FacebookOAuth2ProviderTest,
		skipWhenNotSetup);

void FacebookOAuth2ProviderTest::setUp()
{
	//Obtain auth code by accessing:
	//https://www.facebook.com/v2.9/dialog/oauth?client_id={CLIENT_ID}&redirect_uri={REDIRECT_URI}&response_type=code
	m_authCode = Environment::get("FACEBOOK_AUTH_CODE");
}

void FacebookOAuth2ProviderTest::testVerifyAuthCode()
{
	FacebookOAuth2Provider provider;
	AuthResult info;
	SSLClient::Ptr sslConfig = new SSLClient;

	if (Environment::has("FACEBOOK_CA_LOCATION"))
		sslConfig->setCALocation(Environment::get("FACEBOOK_CA_LOCATION"));
	else
		sslConfig->setCALocation("cert/mozilla-cacert-2018-01-17.pem");

	provider.setSSLConfig(sslConfig);
	provider.setClientId(Environment::get("FACEBOOK_CLIENT_ID"));
	provider.setClientSecret(Environment::get("FACEBOOK_CLIENT_SECRET"));

	AuthCodeCredentials credentials("facebook", m_authCode);

	//MUST be same as redirect uri that was used to get auth code/acces token
	provider.setRedirectUri(
			Environment::get("FACEBOOK_REDIRECT_URI", "https://localhost/"));

	const auto &tokens = provider.exchange(credentials);
	provider.fetch(tokens, info);

	CPPUNIT_ASSERT_MESSAGE("id field has not been obtained",
			!info.providerID().empty());
	CPPUNIT_ASSERT_MESSAGE("email field has not been obtained",
			!info.email().empty());
}

}
