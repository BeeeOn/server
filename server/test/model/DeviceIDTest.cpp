#include <set>
#include <cppunit/extensions/HelperMacros.h>

#include "model/DeviceID.h"

using namespace std;

namespace BeeeOn {

class DeviceIDTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DeviceIDTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testCreate32);
	CPPUNIT_TEST(testCreateFromParts);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST(testParse32);
	CPPUNIT_TEST(testRandom);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testCreate();
	void testCreate32();
	void testCreateFromParts();
	void testParse();
	void testParse32();
	void testRandom();
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

	CPPUNIT_ASSERT(!id.is32bit());
	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xfe, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0x01020304050607UL, id.ident());
	CPPUNIT_ASSERT(id.toString().compare("0xfe01020304050607") == 0);
}

/**
 * Create 32-bit device ID (backwards compatibility).
 */
void DeviceIDTest::testCreate32()
{
	const DeviceID id(0x00000000fe010203UL);

	CPPUNIT_ASSERT(id.is32bit());
	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xfe, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0x00010203, id.ident());
	CPPUNIT_ASSERT(id.toString().compare("0xfe010203") == 0);
}

void DeviceIDTest::testCreateFromParts()
{
	const DeviceID id(0xa0, 0xabcdef01234567UL);

	CPPUNIT_ASSERT(!id.is32bit());
	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xa0, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0xabcdef01234567UL, id.ident());
	CPPUNIT_ASSERT(id.toString().compare("0xa0abcdef01234567") == 0);
}

void DeviceIDTest::testParse()
{
	const DeviceID &id = DeviceID::parse("0xac11223344556677");

	CPPUNIT_ASSERT(!id.is32bit());
	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xac, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0x11223344556677UL, id.ident());
}

/**
 * Parse 32-bit device ID (backwards compatibility).
 */
void DeviceIDTest::testParse32()
{
	const DeviceID &id = DeviceID::parse("0xac112233");

	CPPUNIT_ASSERT(id.is32bit());
	CPPUNIT_ASSERT_EQUAL((uint8_t) 0xac, id.prefix());
	CPPUNIT_ASSERT_EQUAL((uint64_t) 0x112233UL, id.ident());
}

void DeviceIDTest::testRandom()
{
	set<DeviceID> generated;

	for (unsigned int i = 0; i < 1000; ++i) {
		const DeviceID id(DeviceID::random(0x10));
		const set<DeviceID>::size_type count = generated.size();

		CPPUNIT_ASSERT(generated.find(id) == generated.end());

		generated.insert(id);

		CPPUNIT_ASSERT_EQUAL(count + 1, generated.size());
	}
}

}
