#include <cppunit/extensions/HelperMacros.h>

#include "model/Entity.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

namespace BeeeOn {

class EntityCollectionTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(EntityCollectionTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST_SUITE_END();

public:
	EntityCollectionTest()
	{
	}

	void setUp();
	void tearDown();
	void testCreate();
};

CPPUNIT_TEST_SUITE_REGISTRATION(EntityCollectionTest);

/**
 * The TestableEntity provides access to protected members of Entity
 * which are otherwise inaccessible from the outer world.
 */
class TestableEntityCollection : public EntityCollection {
public:
	TestableEntityCollection(const string &s):
		EntityCollection(s)
	{}

	using EntityCollection::get;
};

void EntityCollectionTest::setUp()
{
}

void EntityCollectionTest::tearDown()
{
}

void EntityCollectionTest::testCreate()
{
	TestableEntityCollection data("[0, 1, 10, 11, 44, 59]");
	CPPUNIT_ASSERT(data.size() == 6);
	CPPUNIT_ASSERT(data.get<int>(0) == 0);
	CPPUNIT_ASSERT(data.get<int>(1) == 1);
	CPPUNIT_ASSERT(data.get<int>(2) == 10);
	CPPUNIT_ASSERT(data.get<int>(3) == 11);
	CPPUNIT_ASSERT(data.get<int>(4) == 44);
	CPPUNIT_ASSERT(data.get<int>(5) == 59);
}

}
