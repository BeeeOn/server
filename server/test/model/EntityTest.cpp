#include <cppunit/extensions/HelperMacros.h>

#include "model/Entity.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace Poco::Dynamic;

namespace BeeeOn {

class EntityTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(EntityTest);
	CPPUNIT_TEST(testCreate);
	CPPUNIT_TEST_SUITE_END();

public:
	EntityTest()
	{
	}

	void setUp();
	void tearDown();
	void testCreate();
};

CPPUNIT_TEST_SUITE_REGISTRATION(EntityTest);

/**
 * The TestableEntity provides access to protected members of Entity
 * which are otherwise inaccessible from the outer world.
 */
class TestableEntity : public Entity {
public:
	TestableEntity(const std::string &s):
		Entity(s)
	{}

	using Entity::get;
};

void EntityTest::setUp()
{
}

void EntityTest::tearDown()
{
}

void EntityTest::testCreate()
{
	TestableEntity data("{\"name\": \"John Doe\", \"age\": 24}");
	CPPUNIT_ASSERT(data.get("name").compare("John Doe") == 0);
	CPPUNIT_ASSERT(data.get("age").compare("24") == 0);
	CPPUNIT_ASSERT(data.get<int>("age") == 24);
}

}
