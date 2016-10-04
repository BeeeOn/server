#include <vector>
#include <cppunit/extensions/HelperMacros.h>

#include "server/SessionManager.h"
#include "provider/MockRandomProvider.h"
#include "util/Base64.h"
#include "provider/PocoRandomProvider.h"

using namespace std;

namespace BeeeOn {

class SessionManagerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SessionManagerTest);
	CPPUNIT_TEST(testOpenClose);
	CPPUNIT_TEST(testMaxUserSessions);
	CPPUNIT_TEST(testSessionTimeout);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testOpenClose();
	void testMaxUserSessions();
	void testSessionTimeout();

private:
	SessionManager m_manager;
};

CPPUNIT_TEST_SUITE_REGISTRATION(SessionManagerTest);

void SessionManagerTest::setUp()
{
	// Set how many sessions is user allowed to have
	m_manager.setMaxUserSessions(10);

	// Set session expire time in seconds
	m_manager.setSessionExpireTime(1);
}

void SessionManagerTest::tearDown()
{
}

#define SESSION_ID64 string(\
	"0123456789abcdef"  \
	"0123456789abcdef"  \
	"0123456789abcdef"  \
	"0123456789abcdef")

void SessionManagerTest::testOpenClose()
{
	MockRandomProvider mockRandomProvider;
	mockRandomProvider.setNextRandom(SESSION_ID64);
	InsecureRandomProvider randomProvider;
	randomProvider.setProviderImpl(&mockRandomProvider);

	m_manager.setSecureRandomProvider(&randomProvider);

	UserID userID = UserID::parse("824b4831-6ce4-4614-8e02-8380d6d92f95");
	User user(userID);

	VerifiedIdentity identity(VerifiedIdentityID::random());
	identity.setUser(user);

	SessionID id = m_manager.open(identity);
	CPPUNIT_ASSERT(Base64::decode(id).compare(SESSION_ID64) == 0);

	ExpirableSession::Ptr infoLookup;
	CPPUNIT_ASSERT(m_manager.lookup(id, infoLookup));

	m_manager.close(id);
	CPPUNIT_ASSERT(!m_manager.lookup(id, infoLookup));
}

// Open 11 new sessions where 11th open should throw an exception
void SessionManagerTest::testMaxUserSessions()
{
	PocoRandomProvider pocoProvider;
	UserID userID = UserID::parse("824b4831-6ce4-4614-8e02-8380d6d92f95");
	User user(userID);
	VerifiedIdentity identity(VerifiedIdentityID::random());
	identity.setUser(user);

	std::vector<SessionID> sessions;

	m_manager.setSecureRandomProvider(&pocoProvider);

	for (int i = 0; i < 10; ++i)
		CPPUNIT_ASSERT_NO_THROW(
			sessions.push_back(m_manager.open(identity)));

	CPPUNIT_ASSERT_THROW(m_manager.open(identity), Poco::IllegalStateException);

	// test we can close and open a session so we are sure
	// that the 10 sessions does not last forever
	SessionID last = sessions.back();
	sessions.pop_back();

	CPPUNIT_ASSERT_NO_THROW(m_manager.close(last));
	CPPUNIT_ASSERT_NO_THROW(
		sessions.push_back(m_manager.open(identity)));
}

void SessionManagerTest::testSessionTimeout()
{
	// Sleep for 1 second + a little bit of overhead.
	// Tests whether the session really expired.
	ExpirableSession::Ptr infoLookup;
	PocoRandomProvider pocoProvider;

	m_manager.setSecureRandomProvider(&pocoProvider);

	UserID userID = UserID::parse("824b4831-6ce4-4614-8e02-8380d6d92f95");
	User user(userID);
	VerifiedIdentity identity(VerifiedIdentityID::random());
	identity.setUser(user);

	SessionID id = m_manager.open(identity);

	usleep(1100000);
	CPPUNIT_ASSERT(!m_manager.lookup(id, infoLookup));
}
}
