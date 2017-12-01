#ifndef BEEEON_AUTH_SERVICE_TEST_H
#define BEEEON_AUTH_SERVICE_TEST_H

#include <Poco/Exception.h>

#include <cppunit/extensions/HelperMacros.h>

#include "provider/MockRandomProvider.h"
#include "provider/RandomProvider.h"
#include "server/SessionManagerImpl.h"
#include "service/AuthServiceImpl.h"
#include "provider/PermitAuthProvider.h"
#include "dao/UserDao.h"
#include "dao/IdentityDao.h"
#include "dao/VerifiedIdentityDao.h"
#include "transaction/NoTransactionFactory.h"
#include "transaction/ThreadLocalTransactionManager.h"
#include "util/Base64.h"
#include "l10n/SystemLocaleManager.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class TestableAuthService;

class AuthServiceImplTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(AuthServiceImplTest);
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
	SessionManagerImpl m_manager;
	MockRandomProvider m_mockRandomProvider;
	InsecureRandomProvider m_insecureRandomProvider;
	TestableAuthService *m_service;
	LocaleManager::Ptr m_localeManager;
	SharedPtr<ThreadLocalTransactionManager> m_transactionManager;
};

CPPUNIT_TEST_SUITE_REGISTRATION(AuthServiceImplTest);

class TestableAuthService : public AuthServiceImpl {
public:
	using AuthServiceImpl::createUser;
	using AuthServiceImpl::loginAsNew;
	using AuthServiceImpl::verifyIdentityAndLogin;
};

#define SESSION_ID64 string( \
	"abcdef0123456789"   \
	"abcdef0123456789"   \
	"abcdef0123456789"   \
	"abcdef0123456789")

void AuthServiceImplTest::setUp()
{
	m_userDao.storage().clear();
	m_identityDao.storage().clear();
	m_verifiedIdentityDao.storage().clear();

	m_mockRandomProvider.setNextRandom(SESSION_ID64);
	m_insecureRandomProvider.setProviderImpl(&m_mockRandomProvider);
	m_manager.setSecureRandomProvider(&m_insecureRandomProvider);
	m_manager.setMaxUserSessions(10);
	m_manager.setSessionExpireTime(1 * Timespan::SECONDS);

	m_localeManager = new SystemLocaleManager();

	m_service = new TestableAuthService();
	m_service->setUserDao(&m_userDao);
	m_service->setIdentityDao(&m_identityDao);
	m_service->setVerifiedIdentityDao(&m_verifiedIdentityDao);
	m_service->setSessionManager(&m_manager);
	m_service->setLocaleManager(m_localeManager);

	m_transactionManager = new ThreadLocalTransactionManager;
	m_transactionManager->setFactory(new NoTransactionFactory);
	m_service->setTransactionManager(m_transactionManager);
}

void AuthServiceImplTest::tearDown()
{
	delete m_service;
}

void AuthServiceImplTest::testPermitAuth()
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
		const Session::Ptr session = m_service->login(cred);
		const SessionID id = session->sessionID();
		CPPUNIT_ASSERT(Base64::decode(id).compare(SESSION_ID64) == 0);
		m_service->logout(id);
	} catch(Exception &e) {
		CPPUNIT_FAIL("unexpected exception: " + e.displayText());
	}
}

void AuthServiceImplTest::testCreateUser()
{
	AuthResult result;
	result.setFirstName("Freddie");
	result.setLastName("Mercury");
	result.setLocale("en_US");

	const User &user = m_service->createUser(result);
	CPPUNIT_ASSERT(user.firstName() == "Freddie");
	CPPUNIT_ASSERT(user.lastName() == "Mercury");

	User hasUser(user.id());
	CPPUNIT_ASSERT(m_userDao.fetch(hasUser));
	CPPUNIT_ASSERT(hasUser.firstName() == "Freddie");
	CPPUNIT_ASSERT(hasUser.lastName() == "Mercury");
}

void AuthServiceImplTest::testLoginAsNew()
{
	AuthResult result;
	result.setEmail("freddie@example.org");
	result.setFirstName("Freddie");
	result.setLastName("Mercury");

	Session::Ptr session = m_service->loginAsNew(result);
	CPPUNIT_ASSERT(!session.isNull());

	Identity identity;
	CPPUNIT_ASSERT(m_identityDao.fetchBy(identity,
				"freddie@example.org"));
	CPPUNIT_ASSERT(!identity.id().isNull());
	CPPUNIT_ASSERT(identity.email() == "freddie@example.org");
}

void AuthServiceImplTest::testFirstLoginBySecondProvider()
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

	Session::Ptr session =
		m_service->verifyIdentityAndLogin(result);
	CPPUNIT_ASSERT(!session.isNull());
}

}

#endif
