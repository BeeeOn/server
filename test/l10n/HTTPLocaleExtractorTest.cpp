#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>

#include "cppunit/BetterAssert.h"
#include "l10n/HTTPLocaleExtractor.h"
#include "l10n/LocaleManager.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class TestingHTTPLocaleManager : public LocaleManager {
public:
	Locale parse(const string &input) override
	{
		throw NotImplementedException(__func__);
	}

	Locale chooseBest(const vector<string> &input) override
	{
		m_lastBest = input;
		return Locale();
	}

	vector<string> lastBest() const
	{
		return m_lastBest;
	}

private:
	vector<string> m_lastBest;
};

#define expectBest(manager, vec)                                         \
{                                                                        \
	const vector<string> expect = vec;                               \
	CPPUNIT_ASSERT_EQUAL(expect.size(), manager->lastBest().size()); \
	for (size_t i = 0; i < expect.size(); ++i)                       \
		CPPUNIT_ASSERT_EQUAL(expect[i], manager->lastBest()[i]); \
}

class HTTPLocaleExtractorTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(HTTPLocaleExtractorTest);
	CPPUNIT_TEST(testParseWeight);
	CPPUNIT_TEST(testExtract);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp() override;

	void testParseWeight();
	void testExtract();

private:
	SharedPtr<TestingHTTPLocaleManager> m_manager;
};

CPPUNIT_TEST_SUITE_REGISTRATION(HTTPLocaleExtractorTest);

void HTTPLocaleExtractorTest::setUp()
{
	m_manager = new TestingHTTPLocaleManager;
}

void HTTPLocaleExtractorTest::testParseWeight()
{
	CPPUNIT_ASSERT_EQUAL(0, HTTPLocaleExtractor::parseWeight("0"));
	CPPUNIT_ASSERT_EQUAL(0, HTTPLocaleExtractor::parseWeight("0.0"));
	CPPUNIT_ASSERT_EQUAL(1000, HTTPLocaleExtractor::parseWeight("1"));
	CPPUNIT_ASSERT_EQUAL(1000, HTTPLocaleExtractor::parseWeight("1.0"));
	CPPUNIT_ASSERT_EQUAL(500, HTTPLocaleExtractor::parseWeight("0.5"));
	CPPUNIT_ASSERT_EQUAL(100, HTTPLocaleExtractor::parseWeight("0.1"));
	CPPUNIT_ASSERT_EQUAL(10, HTTPLocaleExtractor::parseWeight("0.01"));
	CPPUNIT_ASSERT_EQUAL(1, HTTPLocaleExtractor::parseWeight("0.001"));

	CPPUNIT_ASSERT_EQUAL(-1, HTTPLocaleExtractor::parseWeight("2"));
	CPPUNIT_ASSERT_EQUAL(-1, HTTPLocaleExtractor::parseWeight("-1"));
	CPPUNIT_ASSERT_EQUAL(-1, HTTPLocaleExtractor::parseWeight(""));
	CPPUNIT_ASSERT_EQUAL(-1, HTTPLocaleExtractor::parseWeight("xxx"));
	CPPUNIT_ASSERT_EQUAL(-1, HTTPLocaleExtractor::parseWeight("q="));
}

void HTTPLocaleExtractorTest::testExtract()
{
	HTTPLocaleExtractor extractor;
	extractor.setLocaleManager(m_manager);

	extractor.extract("en");
	expectBest(m_manager, {"en"});

	extractor.extract("en;qqq;q=;qx");
	expectBest(m_manager, {"en"});

	extractor.extract("fr-CH");
	expectBest(m_manager, {"fr-CH"});

	extractor.extract("fr-CH;xx;qewq");
	expectBest(m_manager, {"fr-CH"});

	extractor.extract("fr;q=0.1, sp;q=0.5, en;q=0.6");
	expectBest(m_manager, vector<string>({"en", "sp", "fr"}));

	extractor.extract("fr;q=0.7;q=0.1, sp;q=0.5, en;q=2;q=0.6;q=");
	expectBest(m_manager, vector<string>({"fr", "en", "sp"}));
}

}
