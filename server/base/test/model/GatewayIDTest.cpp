#include <set>
#include <cppunit/extensions/HelperMacros.h>

#include "model/GatewayID.h"

using namespace std;

namespace BeeeOn {

class GatewayIDTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(GatewayIDTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST(testRandom);
	CPPUNIT_TEST(testLeadingZeros);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testCreate();
	void testParse();
	void testRandom();
	void testLeadingZeros();
};

CPPUNIT_TEST_SUITE_REGISTRATION(GatewayIDTest);

void GatewayIDTest::setUp()
{
}

void GatewayIDTest::tearDown()
{
}

void GatewayIDTest::testCreate()
{
	const GatewayID id(1, 61762224815122);

	CPPUNIT_ASSERT_EQUAL(1, id.version());
	CPPUNIT_ASSERT_EQUAL(61762224815122UL, id.data());
	CPPUNIT_ASSERT(id.toString().compare("1617622248151223") == 0);
}

void GatewayIDTest::testParse()
{
	const GatewayID &id = GatewayID::parse("1617622248151223");

	CPPUNIT_ASSERT_EQUAL(1, id.version());
	CPPUNIT_ASSERT_EQUAL(61762224815122UL, id.data());
	CPPUNIT_ASSERT(id.toString().compare("1617622248151223") == 0);
}

void GatewayIDTest::testRandom()
{
	set<string> used;

	for (int i = 0; i < 10000; ++i) {
		const GatewayID &id = GatewayID::random(1, i);

		CPPUNIT_ASSERT_EQUAL(1, id.version());
		CPPUNIT_ASSERT_EQUAL((size_t) 16, id.toString().size());

		CPPUNIT_ASSERT(used.find(id.toString()) == used.end());
		used.insert(id.toString());
	}
}

void GatewayIDTest::testLeadingZeros()
{
	GatewayID id(1, 1);
	CPPUNIT_ASSERT(id.toString().compare("1000000000000015") == 0);
}

}
