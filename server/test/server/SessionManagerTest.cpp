#include <cppunit/extensions/HelperMacros.h>

#include "server/SessionManager.h"
#include "provider/MockRandomProvider.h"
#include "util/Base64.h"

using namespace std;

namespace BeeeOn {

class SessionManagerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SessionManagerTest);
	CPPUNIT_TEST(testOpenClose);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testOpenClose();

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

	SessionID id = m_manager.open(user);
	CPPUNIT_ASSERT(Base64::decode(id).compare(SESSION_ID64) == 0);

	SessionManager::SessionPtr infoLookup;
	CPPUNIT_ASSERT(m_manager.lookup(id, infoLookup));

	m_manager.close(id);
	CPPUNIT_ASSERT(!m_manager.lookup(id, infoLookup));
}

}
