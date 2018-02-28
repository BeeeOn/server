#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>

#include "cppunit/BetterAssert.h"
#include "server/HTTPCustomHeadersFilter.h"
#include "server/TestingHTTPServerRequest.h"
#include "server/TestingHTTPServerResponse.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

namespace BeeeOn {

class HTTPCustomHeadersFilterTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(HTTPCustomHeadersFilterTest);
	CPPUNIT_TEST(testHeadersForExactMatch);
	CPPUNIT_TEST(testHeadersForPatternMatch);
	CPPUNIT_TEST(testHeadersForNoMatch);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();

	void testHeadersForExactMatch();
	void testHeadersForPatternMatch();
	void testHeadersForNoMatch();

private:
	SharedPtr<TestingHTTPServerRequest> req;
	SharedPtr<TestingHTTPServerResponse> res;
};

CPPUNIT_TEST_SUITE_REGISTRATION(HTTPCustomHeadersFilterTest);

void HTTPCustomHeadersFilterTest::setUp()
{
	res = new TestingHTTPServerResponse;
	req = new TestingHTTPServerRequest(*res);

	req->setVersion(HTTPRequest::HTTP_1_1);
	req->setMethod(HTTPRequest::HTTP_GET);
	req->setURI("/index.html");
	req->set("Host", "example.org");
}

void HTTPCustomHeadersFilterTest::testHeadersForExactMatch()
{
	HTTPCustomHeadersFilter filter;

	filter.setUriPattern("/index.html");
	filter.setHeaders({
		{"Content-Type", "text/plain"},
		{"Connection", "close"},
	});

	CPPUNIT_ASSERT(!res->has("Content-Type"));
	CPPUNIT_ASSERT(!res->has("Connection"));

	filter.apply(*req, *res);

	CPPUNIT_ASSERT(res->has("Content-Type"));
	CPPUNIT_ASSERT_EQUAL("text/plain", res->get("Content-Type"));
	CPPUNIT_ASSERT(res->has("Connection"));
	CPPUNIT_ASSERT_EQUAL("close", res->get("Connection"));
}

void HTTPCustomHeadersFilterTest::testHeadersForPatternMatch()
{
	HTTPCustomHeadersFilter filter;

	filter.setUriPattern("/*.html");
	filter.setHeaders({
		{"Content-Length", "1023"},
		{"Connection", "keep-alive"},
	});

	CPPUNIT_ASSERT(!res->has("Content-Length"));
	CPPUNIT_ASSERT(!res->has("Connection"));

	filter.apply(*req, *res);

	CPPUNIT_ASSERT(res->has("Content-Length"));
	CPPUNIT_ASSERT_EQUAL("1023", res->get("Content-Length"));
	CPPUNIT_ASSERT(res->has("Connection"));
	CPPUNIT_ASSERT_EQUAL("keep-alive", res->get("Connection"));
}

/**
 * Preflight requests with Origin that is not whitelisted must be denied.
 */
void HTTPCustomHeadersFilterTest::testHeadersForNoMatch()
{
	HTTPCustomHeadersFilter filter;

	filter.setUriPattern("/prefix/*.html");
	filter.setHeaders({
		{"Content-Encoding", "gzip"},
		{"Connection", "keep-alive"},
	});

	CPPUNIT_ASSERT(!res->has("Content-Encoding"));
	CPPUNIT_ASSERT(!res->has("Connection"));

	filter.apply(*req, *res);

	CPPUNIT_ASSERT(!res->has("Content-Encoding"));
	CPPUNIT_ASSERT(!res->has("Connection"));
}

}
