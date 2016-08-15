#include <set>
#include <cppunit/extensions/HelperMacros.h>

#include "model/DeviceID.h"

using namespace std;

namespace BeeeOn {

class DeviceIDTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DeviceIDTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testCreateFromParts);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testCreate();
	void testCreateFromParts();
	void testParse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(DeviceIDTest);

void DeviceIDTest::setUp()
{
}

void DeviceIDTest::tearDown()
{
}

void DeviceIDTest::testCreate()
{
	const DeviceID id(0xfe01020304050607UL);

	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xfe, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0x01020304050607UL, id.ident());
	CPPUNIT_ASSERT(id.toString().compare("0xfe01020304050607") == 0);
}

void DeviceIDTest::testCreateFromParts()
{
	const DeviceID id(0xa0, 0xabcdef01234567UL);

	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xa0, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0xabcdef01234567UL, id.ident());
	CPPUNIT_ASSERT(id.toString().compare("0xa0abcdef01234567") == 0);
}

void DeviceIDTest::testParse()
{
	const DeviceID &id = DeviceID::parse("0xac11223344556677");

	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xac, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0x11223344556677UL, id.ident());
}

}
