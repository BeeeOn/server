#ifndef BEEEON_AUTH_SERVICE_TEST_H
#define BEEEON_AUTH_SERVICE_TEST_H

#include <Poco/Exception.h>

#include <cppunit/extensions/HelperMacros.h>

#include "provider/MockRandomProvider.h"
#include "provider/RandomProvider.h"
#include "service/AuthService.h"
#include "provider/PermitAuthProvider.h"
#include "dao/UserDao.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class AuthServiceTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(AuthServiceTest);
	CPPUNIT_TEST(testPermitAuth);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testPermitAuth();
};

CPPUNIT_TEST_SUITE_REGISTRATION(AuthServiceTest);

void AuthServiceTest::setUp()
{
}

void AuthServiceTest::tearDown()
{
}

#define SESSION_ID64 string( \
	"abcdef0123456789"   \
	"abcdef0123456789"   \
	"abcdef0123456789"   \
	"abcdef0123456789")

void AuthServiceTest::testPermitAuth()
{
	MockRandomProvider mockRandomProvider;
	mockRandomProvider.setNextRandom(SESSION_ID64);
	InsecureRandomProvider randomProvider;
	randomProvider.setProviderImpl(&mockRandomProvider);
	SessionManager manager;
	manager.setSecureRandomProvider(&randomProvider);

	MockUserDao dao;
	User::Ptr user(new User(
			"{\"id\": 0, \"email\": \"permit@example.org\"}"));
	dao.storage().insert(make_pair(0, user));

	AuthService service;
	PermitAuthProvider provider;

	service.setUserDao(&dao);
	service.setSessionManager(&manager);
	service.registerProvider(&provider);

	map<string, string> data;
	data["provider"] = "permit";
	data["token"] = "any";
	Credentials cred("", "", data);

	try {
		const string &id = service.login(cred);
		CPPUNIT_ASSERT(id.compare(SESSION_ID64) == 0);
		service.logout(id);
	} catch(Exception &e) {
		CPPUNIT_FAIL("unexpected exception: " + e.displayText());
	}
}

}

#endif
