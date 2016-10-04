#ifndef BEEEON_AUTH_SERVICE_TEST_H
#define BEEEON_AUTH_SERVICE_TEST_H

#include <Poco/Exception.h>

#include <cppunit/extensions/HelperMacros.h>

#include "provider/MockRandomProvider.h"
#include "provider/RandomProvider.h"
#include "service/AuthService.h"
#include "provider/PermitAuthProvider.h"
#include "dao/UserDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "util/Base64.h"

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
	manager.setMaxUserSessions(10);
	manager.setSessionExpireTime(1);

	MockUserDao userDao;
	UserID newID(UUIDGenerator::defaultGenerator().createRandom());
	User::Ptr user(new User(newID));
	user->setEmail("permit@example.org");
	userDao.storage().insert(make_pair(user->id(), user));

	MockIdentityDao identityDao;
	Identity identity;
	identity.setEmail("permit@example.org");
	identity.setUser(*user);
	identityDao.create(identity);

	MockVerifiedIdentityDao verifiedIdentityDao;
	VerifiedIdentity verifiedIdentity;
	verifiedIdentity.setIdentity(identity);
	verifiedIdentity.setProvider("3rd-party");
	verifiedIdentityDao.create(verifiedIdentity);

	AuthService service;
	PermitAuthProvider provider;
	provider.setResultProvider("3rd-party");

	service.setUserDao(&userDao);
	service.setIdentityDao(&identityDao);
	service.setVerifiedIdentityDao(&verifiedIdentityDao);
	service.setSessionManager(&manager);
	service.registerProvider(&provider);

	AuthCodeCredentials cred("permit", "permit@example.org");

	try {
		const string &id = service.login(cred);
		CPPUNIT_ASSERT(Base64::decode(id).compare(SESSION_ID64) == 0);
		service.logout(id);
	} catch(Exception &e) {
		CPPUNIT_FAIL("unexpected exception: " + e.displayText());
	}
}

}

#endif
