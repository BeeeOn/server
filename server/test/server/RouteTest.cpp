#include <cppunit/extensions/HelperMacros.h>

#include "server/Route.h"

namespace BeeeOn {

class RouteTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(RouteTest);
	CPPUNIT_TEST(testExpectMatch);
	CPPUNIT_TEST(testNotExpectMatch);
	CPPUNIT_TEST(testParams);
	CPPUNIT_TEST_SUITE_END();
public:
	RouteTest() {}

	void setUp();
	void tearDown();
	void testExpectMatch();
	void testNotExpectMatch();
	void testParams();
};

CPPUNIT_TEST_SUITE_REGISTRATION(RouteTest);

void RouteTest::setUp()
{
}

void RouteTest::tearDown()
{
}

struct UserData {
	bool success;
};

typedef TRoute<void *, void *, UserData> Route;
typedef TRouteContext<void *, void *, UserData> RouteContext;
typedef Route::Params Params;

void emptyHandler(RouteContext &)
{
}

/**
 * Test paths that successfully match the given patterns.
 * No params are extracted (nor tested) here.
 */
void RouteTest::testExpectMatch()
{
	UserData data = {.success = false};
	Params params;

	Route r0("/", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(r0.match("/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r1("*", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(r1.match("", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(r1.match("/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(r1.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(r1.match("anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r2("/*", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(r2.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r3("/*/path", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(r3.match("/anything/path", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(r3.match("/*/path", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r4("/path", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(r4.match("/path", params));
	CPPUNIT_ASSERT(params.size() == 0);

	// XXX: the /path/ is matched here, is it OK?
	//      We don't care for now.
	params.clear();
	CPPUNIT_ASSERT(r4.match("/path/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r5("/path/longer", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(r5.match("/path/longer", params));
	CPPUNIT_ASSERT(params.size() == 0);
}

/**
 * Test paths the do not match the given patterns.
 * No params are extracted (tested) here.
 */
void RouteTest::testNotExpectMatch()
{
	UserData data = {.success = false};
	Params params;

	Route r0("", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r0.match("", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r0.match("/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r0.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r1("/", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r1.match("", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r1.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r2("/*", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r2.match("", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r2.match("/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r3("/*/*", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r3.match("/*", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r3.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r3.match("/anything/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r3.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r4("/*/path", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r4.match("/path", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r4.match("/path/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r4.match("/path/path1", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r4.match("/path/pat", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r4.match("/path/path/long", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r5("/path", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r5.match("/path/longer", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r6("/path/longer", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r6.match("/path/longer/more", params));
	CPPUNIT_ASSERT(params.size() == 0);
}

/**
 * Test params extraction from URL by patterns.
 */
void RouteTest::testParams()
{
	UserData data = {.success = false};
	Params params;

	Route r0(":name", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r0.match("", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r0.match("/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r0.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	Route r1("/:name", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r1.match("", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(r1.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 1);
	CPPUNIT_ASSERT(params["name"].compare("anything") == 0);

	Route r2("/*/:name", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r2.match("", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r2.match("/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(r2.match("/anything/NAME", params));
	CPPUNIT_ASSERT(params.size() == 1);
	CPPUNIT_ASSERT(params["name"].compare("NAME") == 0);

	Route r3("/:id/*", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(!r3.match("/*", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r3.match("/anything", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!r3.match("/anything/", params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(r3.match("/ID/anything", params));
	CPPUNIT_ASSERT(params.size() == 1);
	CPPUNIT_ASSERT(params["id"].compare("ID") == 0);

	Route r4("/:id/:name/*/:value", emptyHandler);

	params.clear();
	CPPUNIT_ASSERT(r4.match("/ID/NAME/anything/VALUE", params));
	CPPUNIT_ASSERT(params.size() == 3);
	CPPUNIT_ASSERT(params["id"].compare("ID") == 0);
	CPPUNIT_ASSERT(params["name"].compare("NAME") == 0);
	CPPUNIT_ASSERT(params["value"].compare("VALUE") == 0);

	params.clear();
	CPPUNIT_ASSERT(!r4.match("/ID/NAME/anything/long/VALUE", params));
	CPPUNIT_ASSERT(params.size() == 0);
}

}
