#ifndef BEEEON_AUTH_SERVICE_TEST_H
#define BEEEON_AUTH_SERVICE_TEST_H

#include <Poco/Exception.h>

#include <cppunit/extensions/HelperMacros.h>

#include "provider/MockRandomProvider.h"
#include "provider/RandomProvider.h"
#include "service/NotificationService.h"
#include "service/AuthService.h"
#include "provider/PermitAuthProvider.h"
#include "dao/UserDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "util/Base64.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class TestableAuthService;
class MockNotificationService;

class AuthServiceTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(AuthServiceTest);
	CPPUNIT_TEST(testPermitAuth);
	CPPUNIT_TEST(testCreateUser);
	CPPUNIT_TEST(testLoginAsNew);
	CPPUNIT_TEST(testFirstLoginBySecondProvider);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	void testPermitAuth();
	void testCreateUser();
	void testLoginAsNew();
	void testFirstLoginBySecondProvider();

private:
	MockUserDao m_userDao;
	MockIdentityDao m_identityDao;
	MockVerifiedIdentityDao m_verifiedIdentityDao;
	SessionManager m_manager;
	MockRandomProvider m_mockRandomProvider;
	InsecureRandomProvider m_insecureRandomProvider;
	MockNotificationService *m_notificationService;
	TestableAuthService *m_service;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AuthServiceTest);

class TestableAuthService : public AuthService {
public:
	using AuthService::createUser;
	using AuthService::loginAsNew;
	using AuthService::verifyIdentityAndLogin;
};

class MockNotificationService : public NotificationService {
public:
	void notifyFirstLogin(const VerifiedIdentity &identity) override
	{
		m_identity = identity;
	}

	const VerifiedIdentity &lastIdentity()
	{
		return m_identity;
	}

private:
	VerifiedIdentity m_identity;
};

#define SESSION_ID64 string( \
	"abcdef0123456789"   \
	"abcdef0123456789"   \
	"abcdef0123456789"   \
	"abcdef0123456789")

void AuthServiceTest::setUp()
{
	m_userDao.storage().clear();
	m_identityDao.storage().clear();
	m_verifiedIdentityDao.storage().clear();

	m_mockRandomProvider.setNextRandom(SESSION_ID64);
	m_insecureRandomProvider.setProviderImpl(&m_mockRandomProvider);
	m_manager.setSecureRandomProvider(&m_insecureRandomProvider);
	m_manager.setMaxUserSessions(10);
	m_manager.setSessionExpireTime(1);

	m_notificationService = new MockNotificationService();

	m_service = new TestableAuthService();
	m_service->setUserDao(&m_userDao);
	m_service->setIdentityDao(&m_identityDao);
	m_service->setVerifiedIdentityDao(&m_verifiedIdentityDao);
	m_service->setSessionManager(&m_manager);
	m_service->setNotificationService(m_notificationService);
}

void AuthServiceTest::tearDown()
{
	delete m_service;
	delete m_notificationService;
}

void AuthServiceTest::testPermitAuth()
{
	UserID newID(UUIDGenerator::defaultGenerator().createRandom());
	User::Ptr user(new User(newID));
	m_userDao.storage().insert(make_pair(user->id(), user));

	Identity identity;
	identity.setEmail("permit@example.org");
	m_identityDao.create(identity);

	VerifiedIdentity verifiedIdentity;
	verifiedIdentity.setIdentity(identity);
	verifiedIdentity.setUser(*user);
	verifiedIdentity.setProvider("3rd-party");
	m_verifiedIdentityDao.create(verifiedIdentity);

	PermitAuthProvider provider;
	provider.setResultProvider("3rd-party");

	m_service->registerProvider(&provider);

	AuthCodeCredentials cred("permit", "permit@example.org");

	try {
		const ExpirableSession::Ptr session = m_service->login(cred);
		const SessionID id = session->sessionID();
		CPPUNIT_ASSERT(Base64::decode(id).compare(SESSION_ID64) == 0);
		m_service->logout(id);
	} catch(Exception &e) {
		CPPUNIT_FAIL("unexpected exception: " + e.displayText());
	}
}

void AuthServiceTest::testCreateUser()
{
	AuthResult result;
	result.setFirstName("Freddie");
	result.setLastName("Mercury");

	const User &user = m_service->createUser(result);
	CPPUNIT_ASSERT(user.firstName() == "Freddie");
	CPPUNIT_ASSERT(user.lastName() == "Mercury");

	User hasUser(user.id());
	CPPUNIT_ASSERT(m_userDao.fetch(hasUser));
	CPPUNIT_ASSERT(hasUser.firstName() == "Freddie");
	CPPUNIT_ASSERT(hasUser.lastName() == "Mercury");
}

void AuthServiceTest::testLoginAsNew()
{
	AuthResult result;
	result.setEmail("freddie@example.org");
	result.setFirstName("Freddie");
	result.setLastName("Mercury");

	ExpirableSession::Ptr session = m_service->loginAsNew(result);
	CPPUNIT_ASSERT(!session.isNull());

	Identity identity;
	CPPUNIT_ASSERT(m_identityDao.fetchBy(identity,
				"freddie@example.org"));
	CPPUNIT_ASSERT(!identity.id().isNull());
	CPPUNIT_ASSERT(identity.email() == "freddie@example.org");

	const VerifiedIdentity &verifiedIdentity =
		m_notificationService->lastIdentity();

	CPPUNIT_ASSERT(!verifiedIdentity.id().isNull());
	CPPUNIT_ASSERT(verifiedIdentity.id() == session->identityID());
	CPPUNIT_ASSERT(verifiedIdentity.identity().id() == identity.id());
	CPPUNIT_ASSERT(!verifiedIdentity.user().id().isNull());
	CPPUNIT_ASSERT(verifiedIdentity.user().firstName() == "Freddie");
	CPPUNIT_ASSERT(verifiedIdentity.user().lastName() == "Mercury");
	CPPUNIT_ASSERT(verifiedIdentity.email() == "freddie@example.org");
}

void AuthServiceTest::testFirstLoginBySecondProvider()
{
	User user;
	user.setFirstName("Freddie");
	user.setLastName("Mercury");
	m_userDao.create(user);

	Identity identity;
	identity.setEmail("freddie@example.org");
	m_identityDao.create(identity);

	VerifiedIdentity firstProvider;
	firstProvider.setIdentity(identity);
	firstProvider.setUser(user);
	firstProvider.setProvider("first-provider");
	m_verifiedIdentityDao.create(firstProvider);

	AuthResult result;
	result.setEmail("freddie@example.org");
	result.setProvider("second-provider");
	result.setFirstName("Freddie 2");
	result.setLastName("Mercury 2");

	ExpirableSession::Ptr session =
		m_service->verifyIdentityAndLogin(result);
	CPPUNIT_ASSERT(!session.isNull());

	const VerifiedIdentity &verifiedIdentity =
		m_notificationService->lastIdentity();

	CPPUNIT_ASSERT(!verifiedIdentity.id().isNull());
	CPPUNIT_ASSERT(verifiedIdentity.id() == session->identityID());
	CPPUNIT_ASSERT(verifiedIdentity.identity().id() == identity.id());
	CPPUNIT_ASSERT(verifiedIdentity.user().id() == user.id());
	CPPUNIT_ASSERT(verifiedIdentity.user().firstName() == "Freddie");
	CPPUNIT_ASSERT(verifiedIdentity.user().lastName() == "Mercury");
	CPPUNIT_ASSERT(verifiedIdentity.email() == "freddie@example.org");
}

}

#endif
