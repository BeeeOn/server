#include <Poco/Exception.h>
#include <Poco/Environment.h>

#include <cppunit/extensions/HelperMacros.h>

#include "provider/GoogleAuthProvider.h"
#include "provider/AuthProvider.h"
#include "cppunit/SkippableAutoRegisterSuite.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

bool skipWhenNoAuthCode()
{
	return !Environment::has("GOOGLE_AUTH_CODE");
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

	CPPUNIT_ASSERT_MESSAGE("failed to authenticate user",
			provider.verifyAuthCode(googleAuthCode, info));

	CPPUNIT_ASSERT_MESSAGE("missing email field",
			!info.email().empty());
	CPPUNIT_ASSERT_MESSAGE("missing google_id field",
			!info.providerID().empty());
}
}


