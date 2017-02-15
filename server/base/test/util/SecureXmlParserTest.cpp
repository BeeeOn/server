#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/AutoPtr.h>
#include <Poco/DOM/DOMParser.h>
#include <Poco/DOM/Document.h>

#include "util/SecureXmlParser.h"
#include "cppunit/BetterAssert.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;

namespace BeeeOn {

class SecureXmlParserTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SecureXmlParserTest);
	CPPUNIT_TEST(testGenericEntityExpansionVulnerable);
	CPPUNIT_TEST(testGenericEntityExpansionSecured);
	CPPUNIT_TEST(testRecursiveEntityExpansionVulnerable);
	CPPUNIT_TEST(testRecursiveEntityExpansionSecured);
	CPPUNIT_TEST_SUITE_END();
public:
	void testGenericEntityExpansionVulnerable();
	void testGenericEntityExpansionSecured();
	void testRecursiveEntityExpansionVulnerable();
	void testRecursiveEntityExpansionSecured();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SecureXmlParserTest);


static const string GENERIC_ENTITY_EXPANSION_ATTACK  = 
	"<!DOCTYPE s ["
	"<!ENTITY x \"VERY LONG CONTENT\">"
	"]>"
	"<test>&x;&x;&x;</test>";

/**
 * Test the parser is vulnerable to generic entity expansion.
 */
void SecureXmlParserTest::testGenericEntityExpansionVulnerable()
{
	DOMParser parser;
	const AutoPtr<Document> doc = parser.parseString(
			GENERIC_ENTITY_EXPANSION_ATTACK);

	const Element *root = doc->documentElement();
	const string &value = root->innerText();

	const string expect = "VERY LONG CONTENT"
		"VERY LONG CONTENT" "VERY LONG CONTENT";
	CPPUNIT_ASSERT_EQUAL(expect, value);
}

void SecureXmlParserTest::testGenericEntityExpansionSecured()
{
	SecureXmlParser parser;

	CPPUNIT_ASSERT_THROW(parser.parse(
		GENERIC_ENTITY_EXPANSION_ATTACK),
			InvalidArgumentException);
}

static const string RECURSIVE_ENTITY_EXPANSION_ATTACK =
		"<!DOCTYPE s ["
		"<!ENTITY x0 \":)\">"
		"<!ENTITY x1 \"&x0;&x0;\">"
		"<!ENTITY x2 \"&x1;&x1;\">"
		"]>"
		"<test>&x2;</test>";

/**
 * Test the parser is vulnerable to recursive entity expansion.
 */
void SecureXmlParserTest::testRecursiveEntityExpansionVulnerable()
{
	DOMParser parser;
	const AutoPtr<Document> doc = parser.parseString(
			RECURSIVE_ENTITY_EXPANSION_ATTACK);

	const Element *root = doc->documentElement();
	const string &value = root->innerText();

	const string expect = ":):):):)";
	CPPUNIT_ASSERT_EQUAL(expect, value);
}

void SecureXmlParserTest::testRecursiveEntityExpansionSecured()
{
	SecureXmlParser parser;

	CPPUNIT_ASSERT_THROW(parser.parse(
		RECURSIVE_ENTITY_EXPANSION_ATTACK),
			InvalidArgumentException);
}

}
