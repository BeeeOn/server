#include <cppunit/extensions/HelperMacros.h>

#include "model/GatewayID.h"

namespace BeeeOn {

class GatewayIDTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(GatewayIDTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST(testParse);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();
	void testCreate();
	void testParse();
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

}
