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

	SessionManager::Info info;
	info.insert(make_pair(SessionManager::INFO_EMAIL,
				"test@exmaple.org"));

	SessionID id = manager.open(info);
	CPPUNIT_ASSERT(Base64::decode(id).compare(SESSION_ID64) == 0);

	SessionManager::Info infoLookup;
	CPPUNIT_ASSERT(manager.lookup(id, infoLookup));

	manager.close(id);
	CPPUNIT_ASSERT(!manager.lookup(id, infoLookup));
}

}
