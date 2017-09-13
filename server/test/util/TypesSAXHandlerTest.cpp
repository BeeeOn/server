#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/XML/XMLException.h>
#include <Poco/SAX/SAXParser.h>

#include "cppunit/BetterAssert.h"
#include "util/TypesSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;

namespace BeeeOn {

class TypesSAXHandlerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TypesSAXHandlerTest);
	CPPUNIT_TEST(testParseSimple);
	CPPUNIT_TEST(testParseWithRange);
	CPPUNIT_TEST(testParseMissingId);
	CPPUNIT_TEST(testParseMissingName);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testParseSimple();
	void testParseWithRange();
	void testParseMissingId();
	void testParseMissingName();

private:
	SAXParser m_parser;
	TypesSAXHandler *m_handler;
};

CPPUNIT_TEST_SUITE_REGISTRATION(TypesSAXHandlerTest);

void TypesSAXHandlerTest::setUp()
{
	m_handler = new TypesSAXHandler();
	m_parser.setContentHandler(m_handler);
}

void TypesSAXHandlerTest::tearDown()
{
	m_parser.setContentHandler(NULL);
	delete m_handler;
}

void TypesSAXHandlerTest::testParseSimple()
{
	CPPUNIT_ASSERT_NO_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"0x01\">"
			"    <name>NAME1</name>"
			"  </type>"
			"  <type id=\"0x02\">"
			"    <name>NAME2</name>"
			"    <unit>&#176;C</unit>"
			"  </type>"
			"  <type id=\"0x03\">"
			"    <name>NAME3</name>"
			"  </type>"
			"</types>")
	);

	for (auto it : *m_handler) {
		if (it.id() == TypeInfoID::parse("0x01")) {
			CPPUNIT_ASSERT_EQUAL(string("NAME1"), it.name());
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
		}
		else if (it.id() == TypeInfoID::parse("0x02")) {
			CPPUNIT_ASSERT_EQUAL(string("NAME2"), it.name());
			CPPUNIT_ASSERT_EQUAL(string("°C"), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
		}
		else if (it.id() == TypeInfoID::parse("0x03")) {
			CPPUNIT_ASSERT_EQUAL(string("NAME3"), it.name());
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
		}
		else {
			CPPUNIT_ASSERT(false);
		}
	}
}

void TypesSAXHandlerTest::testParseWithRange()
{
	CPPUNIT_ASSERT_NO_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"0x01\">"
			"    <name>NAME1</name>"
			"    <range min=\"-10\" />"
			"  </type>"
			"  <type id=\"0x02\">"
			"    <name>NAME2</name>"
			"    <unit>&#176;C</unit>"
			"    <range max=\"55\" />"
			"  </type>"
			"  <type id=\"0x03\">"
			"    <name>NAME3</name>"
			"    <range step=\"6\" />"
			"  </type>"
			"  <type id=\"0x04\">"
			"    <name>NAME4</name>"
			"    <range min=\"0.1\" max=\"0.5\" step=\"0.1\" />"
			"  </type>"
			"</types>")
	);

	for (auto it : *m_handler) {
		if (it.id() == TypeInfoID::parse("0x01")) {
			CPPUNIT_ASSERT_EQUAL(string("NAME1"), it.name());
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(it.range().isValid());
			CPPUNIT_ASSERT(it.range().hasMin());
			CPPUNIT_ASSERT_EQUAL(-10.0, it.range().min());
			CPPUNIT_ASSERT(!it.range().hasMax());
			CPPUNIT_ASSERT(!it.range().hasStep());
		}
		else if (it.id() == TypeInfoID::parse("0x02")) {
			CPPUNIT_ASSERT_EQUAL(string("NAME2"), it.name());
			CPPUNIT_ASSERT_EQUAL(string("°C"), it.unit());
			CPPUNIT_ASSERT(it.range().isValid());
			CPPUNIT_ASSERT(!it.range().hasMin());
			CPPUNIT_ASSERT(it.range().hasMax());
			CPPUNIT_ASSERT_EQUAL(55.0, it.range().max());
			CPPUNIT_ASSERT(!it.range().hasStep());
		}
		else if (it.id() == TypeInfoID::parse("0x03")) {
			CPPUNIT_ASSERT_EQUAL(string("NAME3"), it.name());
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(it.range().isValid());
			CPPUNIT_ASSERT(!it.range().hasMin());
			CPPUNIT_ASSERT(!it.range().hasMax());
			CPPUNIT_ASSERT(it.range().hasStep());
			CPPUNIT_ASSERT_EQUAL(6.0, it.range().step());
		}
		else if (it.id() == TypeInfoID::parse("0x04")) {
			CPPUNIT_ASSERT_EQUAL(string("NAME4"), it.name());
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(it.range().isValid());
			CPPUNIT_ASSERT(it.range().hasMin());
			CPPUNIT_ASSERT_EQUAL(0.1, it.range().min());
			CPPUNIT_ASSERT(it.range().hasMax());
			CPPUNIT_ASSERT_EQUAL(0.5, it.range().max());
			CPPUNIT_ASSERT(it.range().hasStep());
			CPPUNIT_ASSERT_EQUAL(0.1, it.range().step());
		}
		else {
			CPPUNIT_ASSERT(false);
		}
	}

}

void TypesSAXHandlerTest::testParseMissingId()
{
	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type>"
		"    <name>NAME1</name>"
		"  </type>"
		"</types>"),
		XMLException);
}

void TypesSAXHandlerTest::testParseMissingName()
{
	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type id=\"0x01\" />"
		"</types>"),
		XMLException);
}

}
