#include <Poco/URI.h>

#include <cppunit/extensions/HelperMacros.h>

#include "rest/UriPattern.h"

using namespace Poco;

namespace BeeeOn {

class UriPatternTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(UriPatternTest);
	CPPUNIT_TEST(testExpectMatch);
	CPPUNIT_TEST(testNotExpectMatch);
	CPPUNIT_TEST(testParams);
	CPPUNIT_TEST_SUITE_END();
public:
	UriPatternTest() {}

	void testExpectMatch();
	void testNotExpectMatch();
	void testParams();
};

CPPUNIT_TEST_SUITE_REGISTRATION(UriPatternTest);

/**
 * Test paths that successfully match the given patterns.
 * No params are extracted (nor tested) here.
 */
void UriPatternTest::testExpectMatch()
{
	UriPattern::Params params;

	UriPattern p0("/", {});

	params.clear();
	CPPUNIT_ASSERT(p0.match(URI("/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p1("*", {});

	params.clear();
	CPPUNIT_ASSERT(p1.match(URI(""), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(p1.match(URI("/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(p1.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(p1.match(URI("anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p2("/*", {});

	params.clear();
	CPPUNIT_ASSERT(p2.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p3("/*/path", {});

	params.clear();
	CPPUNIT_ASSERT(p3.match(URI("/anything/path"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(p3.match(URI("/*/path"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p4("/path", {});

	params.clear();
	CPPUNIT_ASSERT(p4.match(URI("/path"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	// XXX: the /path/ is matched here, is it OK?
	//      We don't care for now.
	params.clear();
	CPPUNIT_ASSERT(p4.match(URI("/path/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p5("/path/longer", {});

	params.clear();
	CPPUNIT_ASSERT(p5.match(URI("/path/longer"), params));
	CPPUNIT_ASSERT(params.size() == 0);
}

/**
 * Test paths the do not match the given patterns.
 * No params are extracted (tested) here.
 */
void UriPatternTest::testNotExpectMatch()
{
	UriPattern::Params params;

	UriPattern p0("", {});

	params.clear();
	CPPUNIT_ASSERT(!p0.match(URI(""), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p0.match(URI("/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p0.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p1("/", {});

	params.clear();
	CPPUNIT_ASSERT(!p1.match(URI(""), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p1.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p2("/*", {});

	params.clear();
	CPPUNIT_ASSERT(!p2.match(URI(""), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p2.match(URI("/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p3("/*/*", {});

	params.clear();
	CPPUNIT_ASSERT(!p3.match(URI("/*"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p3.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p3.match(URI("/anything/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p3.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p4("/*/path", {});

	params.clear();
	CPPUNIT_ASSERT(!p4.match(URI("/path"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p4.match(URI("/path/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p4.match(URI("/path/path1"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p4.match(URI("/path/pat"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p4.match(URI("/path/path/long"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p5("/path", {});

	params.clear();
	CPPUNIT_ASSERT(!p5.match(URI("/path/longer"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p6("/path/longer", {});

	params.clear();
	CPPUNIT_ASSERT(!p6.match(URI("/path/longer/more"), params));
	CPPUNIT_ASSERT(params.size() == 0);
}

/**
 * Test params extraction from URL by patterns.
 */
void UriPatternTest::testParams()
{
	UriPattern::Params params;

	UriPattern p0("$name", {"name"});

	params.clear();
	CPPUNIT_ASSERT(!p0.match(URI(""), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p0.match(URI("/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p0.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	UriPattern p1("/$name", {"name"});

	params.clear();
	CPPUNIT_ASSERT(!p1.match(URI(""), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(p1.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 1);
	CPPUNIT_ASSERT(params["name"] == "anything");

	UriPattern p2("/*/$name", {"name"});

	params.clear();
	CPPUNIT_ASSERT(!p2.match(URI(""), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p2.match(URI("/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(p2.match(URI("/anything/NAME"), params));
	CPPUNIT_ASSERT(params.size() == 1);
	CPPUNIT_ASSERT(params["name"] == "NAME");

	UriPattern p3("/$id/*", {"id"});

	params.clear();
	CPPUNIT_ASSERT(!p3.match(URI("/*"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p3.match(URI("/anything"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(!p3.match(URI("/anything/"), params));
	CPPUNIT_ASSERT(params.size() == 0);

	params.clear();
	CPPUNIT_ASSERT(p3.match(URI("/ID/anything"), params));
	CPPUNIT_ASSERT(params.size() == 1);
	CPPUNIT_ASSERT(params["id"] == "ID");

	UriPattern p4("/$id/$name/*/$value", {"id", "name", "value"});

	params.clear();
	CPPUNIT_ASSERT(p4.match(URI("/ID/NAME/anything/VALUE"), params));
	CPPUNIT_ASSERT(params.size() == 3);
	CPPUNIT_ASSERT(params["id"] == "ID");
	CPPUNIT_ASSERT(params["name"] == "NAME");
	CPPUNIT_ASSERT(params["value"] == "VALUE");

	params.clear();
	CPPUNIT_ASSERT(!p4.match(URI("/ID/NAME/anything/long/VALUE"), params));
	CPPUNIT_ASSERT(params.size() == 0);
}

}
