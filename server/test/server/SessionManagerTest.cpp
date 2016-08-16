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
};

CPPUNIT_TEST_SUITE_REGISTRATION(SessionManagerTest);

void SessionManagerTest::setUp()
{
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

	SessionManager manager;
	manager.setSecureRandomProvider(&randomProvider);

	User user;
	user.setEmail("test@example.org");

	SessionID id = manager.open(user);
	CPPUNIT_ASSERT(Base64::decode(id).compare(SESSION_ID64) == 0);

	User userLookup;
	CPPUNIT_ASSERT(manager.lookup(id, userLookup));

	manager.close(id);
	CPPUNIT_ASSERT(!manager.lookup(id, userLookup));
}

}
