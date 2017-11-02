#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>

#include "cppunit/BetterAssert.h"
#include "server/CORSFilter.h"
#include "server/TestingHTTPServerRequest.h"
#include "server/TestingHTTPServerResponse.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;

namespace BeeeOn {

class CORSFilterTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(CORSFilterTest);
	CPPUNIT_TEST(testPreflightWithoutOrigin);
	CPPUNIT_TEST(testPreflightDisallowedOrigin);
	CPPUNIT_TEST(testPreflightNoSettings);
	CPPUNIT_TEST(testPreflight);
	CPPUNIT_TEST(testOriginNotRequired);
	CPPUNIT_TEST(testDisallowedOrigin);
	CPPUNIT_TEST(testDisallowedMethod);
	CPPUNIT_TEST(testDisallowedHeader);
	CPPUNIT_TEST(testPassingRequest);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();

	void testPreflightWithoutOrigin();
	void testPreflightDisallowedOrigin();
	void testPreflightNoSettings();
	void testPreflight();
	void testOriginNotRequired();
	void testDisallowedOrigin();
	void testDisallowedMethod();
	void testDisallowedHeader();
	void testPassingRequest();

private:
	SharedPtr<TestingHTTPServerRequest> req;
	SharedPtr<TestingHTTPServerResponse> res;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CORSFilterTest);

void CORSFilterTest::setUp()
{
	res = new TestingHTTPServerResponse;
	req = new TestingHTTPServerRequest(*res);

	req->setVersion(HTTPRequest::HTTP_1_1);
	req->setURI("/index.html");
	req->set("Host", "example.org");
}

/**
 * Preflight requests must always have the Origin header.
 */
void CORSFilterTest::testPreflightWithoutOrigin()
{
	CORSFilter filter;

	req->setMethod(HTTPRequest::HTTP_OPTIONS);

	CPPUNIT_ASSERT_THROW(filter.apply(*req, *res), InvalidArgumentException);
}

/**
 * Preflight requests with Origin that is not whitelisted must be denied.
 */
void CORSFilterTest::testPreflightDisallowedOrigin()
{
	CORSFilter filter;
	filter.setAllowedOrigins({"thirdparty.com"});

	req->setMethod(HTTPRequest::HTTP_OPTIONS);
	req->set("Origin", "mysuper.com");

	CPPUNIT_ASSERT_THROW(filter.apply(*req, *res), InvalidArgumentException);

	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Credentials"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Max-Age"));
}

/**
 * The default settings of the CORSFilter are handled for preflights as follows:
 *
 * - all origins are allowed, the current Origin is returned in the response
 * - all methods and headers are allowed but none is returned
 * - credentials are disabled
 * - max age is 10 seconds
 */
void CORSFilterTest::testPreflightNoSettings()
{
	CORSFilter filter;

	req->setMethod(HTTPRequest::HTTP_OPTIONS);
	req->set("Origin", "thirdparty.com");

	CPPUNIT_ASSERT_NO_THROW(filter.apply(*req, *res));

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT_EQUAL("thirdparty.com", res->get("Access-Control-Allow-Origin"));

	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Credentials"));

	CPPUNIT_ASSERT(res->has("Access-Control-Max-Age"));
	CPPUNIT_ASSERT_EQUAL("10", res->get("Access-Control-Max-Age"));
}

/**
 * Test a preflight request that pass the checks. Thus:
 *
 * - the passing Origin is returned in Access-Control-Allow-Origin header
 * - all allowed methods are given in Access-Control-Allow-Methods
 * - all allowed headers are given in Access-Control-Allow-Headers
 * - all exposed headers are given in Access-Control-Expose-Headers
 * - Access-Control-Allow-Credentials is true if allowed
 * - Access-Control-Max-Age is returned as configured
 */
void CORSFilterTest::testPreflight()
{
	CORSFilter filter;
	filter.setAllowedOrigins({"thirdparty.com"});
	filter.setAllowedMethods({"GET", "POST", "DELETE"});
	filter.setAllowedHeaders({"X-Requested-By", "XXX"});
	filter.setExposeHeaders({"Set-Cookie"});
	filter.setPreflightMaxAge(10 * Timespan::MINUTES);
	filter.setAllowCredentials(true);

	req->setMethod(HTTPRequest::HTTP_OPTIONS);
	req->set("Origin", "thirdparty.com");

	CPPUNIT_ASSERT_NO_THROW(filter.apply(*req, *res));

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT_EQUAL("thirdparty.com", res->get("Access-Control-Allow-Origin"));

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT_EQUAL("DELETE, GET, POST", res->get("Access-Control-Allow-Methods"));

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT_EQUAL("X-Requested-By, XXX", res->get("Access-Control-Allow-Headers"));

	CPPUNIT_ASSERT(res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT_EQUAL("Set-Cookie", res->get("Access-Control-Expose-Headers"));

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Credentials"));
	CPPUNIT_ASSERT_EQUAL("true", res->get("Access-Control-Allow-Credentials"));

	CPPUNIT_ASSERT(res->has("Access-Control-Max-Age"));
	CPPUNIT_ASSERT_EQUAL("600", res->get("Access-Control-Max-Age"));
}

/**
 * When the requireOrigin preset is false, then requests without the Origin header
 * are not filtered according to the CORS rules. Everything passes the filter.
 */
void CORSFilterTest::testOriginNotRequired()
{
	CORSFilter filter;
	filter.setRequireOrigin(false);

	req->setMethod(HTTPRequest::HTTP_GET);

	CPPUNIT_ASSERT_NO_THROW(filter.apply(*req, *res));

	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Credentials"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Max-Age"));
}

/**
 * Non-preflight requests with a non-whitelisted origin are denied.
 */
void CORSFilterTest::testDisallowedOrigin()
{
	CORSFilter filter;
	filter.setAllowedOrigins({"example.org"});

	req->setMethod(HTTPRequest::HTTP_GET);
	req->set("Origin", "thirdparty.com");

	CPPUNIT_ASSERT_THROW(filter.apply(*req, *res), InvalidArgumentException);

	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Credentials"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Max-Age"));
}

/**
 * Non-preflight requests with a non-whitelisted method are denied.
 */
void CORSFilterTest::testDisallowedMethod()
{
	CORSFilter filter;
	filter.setAllowedOrigins({"thirdparty.com"});
	filter.setAllowedMethods({"POST"});

	req->setMethod(HTTPRequest::HTTP_GET);
	req->set("Origin", "thirdparty.com");

	CPPUNIT_ASSERT_THROW(filter.apply(*req, *res), InvalidArgumentException);

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT_EQUAL("thirdparty.com", res->get("Access-Control-Allow-Origin"));

	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Credentials"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Max-Age"));
}

/**
 * Non-preflight requests with a non-whitelisted header are denied.
 */
void CORSFilterTest::testDisallowedHeader()
{
	CORSFilter filter;
	filter.setAllowedOrigins({"thirdparty.com"});
	filter.setAllowedMethods({"GET"});

	req->setMethod(HTTPRequest::HTTP_GET);
	req->set("Origin", "thirdparty.com");
	req->set("X-Requested-by", "1212312");

	CPPUNIT_ASSERT_THROW(filter.apply(*req, *res), InvalidArgumentException);

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT_EQUAL("thirdparty.com", res->get("Access-Control-Allow-Origin"));

	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Credentials"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Max-Age"));
}

void CORSFilterTest::testPassingRequest()
{
	CORSFilter filter;
	filter.setAllowedOrigins({"thirdparty.com"});
	filter.setAllowedMethods({"GET"});
	filter.setAllowedHeaders({"X-Requested-By"});

	req->setMethod(HTTPRequest::HTTP_GET);
	req->set("Origin", "thirdparty.com");
	req->set("X-Requested-By", "1212312");

	CPPUNIT_ASSERT_NO_THROW(filter.apply(*req, *res));

	CPPUNIT_ASSERT(res->has("Access-Control-Allow-Origin"));
	CPPUNIT_ASSERT_EQUAL("thirdparty.com", res->get("Access-Control-Allow-Origin"));

	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Methods"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Expose-Headers"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Allow-Credentials"));
	CPPUNIT_ASSERT(!res->has("Access-Control-Max-Age"));
}

}
