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
	CPPUNIT_TEST(testParseWithValues);
	CPPUNIT_TEST(testParseHasBothValuesAndRange);
	CPPUNIT_TEST(testParseInvalidValues);
	CPPUNIT_TEST(testParseWithLevels);
	CPPUNIT_TEST(testParseOverlappingLevels);
	CPPUNIT_TEST(testParseMissingId);
	CPPUNIT_TEST(testParseMultipleUnits);
	CPPUNIT_TEST(testParseWrongFirstUnit);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testParseSimple();
	void testParseWithRange();
	void testParseWithValues();
	void testParseHasBothValuesAndRange();
	void testParseInvalidValues();
	void testParseWithLevels();
	void testParseOverlappingLevels();
	void testParseMissingId();
	void testParseMultipleUnits();
	void testParseWrongFirstUnit();

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
			"  <type id=\"temperature\">"
			"  </type>"
			"  <type id=\"humidity\">"
			"    <unit>&#176;C</unit>"
			"  </type>"
			"  <type id=\"battery\">"
			"  </type>"
			"</types>")
	);

	for (auto it : *m_handler) {
		if (it.id() == TypeInfoID::parse("temperature")) {
			CPPUNIT_ASSERT_EQUAL(string("temperature"), it.name());
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
		}
		else if (it.id() == TypeInfoID::parse("humidity")) {
			CPPUNIT_ASSERT_EQUAL(string("humidity"), it.name());
			CPPUNIT_ASSERT_EQUAL(string("°C"), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
		}
		else if (it.id() == TypeInfoID::parse("battery")) {
			CPPUNIT_ASSERT_EQUAL(string("battery"), it.name());
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
			"  <type id=\"temperature\">"
			"    <range min=\"-10\" />"
			"  </type>"
			"  <type id=\"humidity\">"
			"    <unit>&#176;C</unit>"
			"    <range max=\"55\" />"
			"  </type>"
			"  <type id=\"on_off\">"
			"    <range step=\"6\" />"
			"  </type>"
			"  <type id=\"rssi\">"
			"    <range min=\"0.1\" max=\"0.5\" step=\"0.1\" />"
			"  </type>"
			"</types>")
	);

	for (auto it : *m_handler) {
		if (it.id() == TypeInfoID::parse("temperature")) {
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(it.range().isValid());
			CPPUNIT_ASSERT(it.range().hasMin());
			CPPUNIT_ASSERT_EQUAL(-10.0, it.range().min());
			CPPUNIT_ASSERT(!it.range().hasMax());
			CPPUNIT_ASSERT(!it.range().hasStep());
		}
		else if (it.id() == TypeInfoID::parse("humidity")) {
			CPPUNIT_ASSERT_EQUAL(string("°C"), it.unit());
			CPPUNIT_ASSERT(it.range().isValid());
			CPPUNIT_ASSERT(!it.range().hasMin());
			CPPUNIT_ASSERT(it.range().hasMax());
			CPPUNIT_ASSERT_EQUAL(55.0, it.range().max());
			CPPUNIT_ASSERT(!it.range().hasStep());
		}
		else if (it.id() == TypeInfoID::parse("on_off")) {
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(it.range().isValid());
			CPPUNIT_ASSERT(!it.range().hasMin());
			CPPUNIT_ASSERT(!it.range().hasMax());
			CPPUNIT_ASSERT(it.range().hasStep());
			CPPUNIT_ASSERT_EQUAL(6.0, it.range().step());
		}
		else if (it.id() == TypeInfoID::parse("rssi")) {
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

void TypesSAXHandlerTest::testParseWithValues()
{
	CPPUNIT_ASSERT_NO_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"on_off\">"
			"    <values>"
			"      <value equals=\"0\">zero</value>"
			"      <value equals=\"1\">one</value>"
			"    </values>"
			"  </type>"
			"  <type id=\"enum\">"
			"    <unit>&#176;C</unit>"
			"    <values>"
			"      <value equals=\"0\">zero</value>"
			"      <value equals=\"2\">two</value>"
			"      <value equals=\"4\">four</value>"
			"    </values>"
			"  </type>"
			"  <type id=\"availability\">"
			"    <values>"
			"      <value equals=\"-1\">minus one</value>"
			"      <value equals=\"-2\">minus two</value>"
			"      <value equals=\"-3\">minus three</value>"
			"    </values>"
			"  </type>"
			"</types>")
	);

	for (auto it : *m_handler) {
		if (it.id() == TypeInfoID::parse("on_off")) {
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
			CPPUNIT_ASSERT_EQUAL(2, it.values().size());
			CPPUNIT_ASSERT_EQUAL("zero", it.values().at(0));
			CPPUNIT_ASSERT_EQUAL("one", it.values().at(1));
		}
		else if (it.id() == TypeInfoID::parse("enum")) {
			CPPUNIT_ASSERT_EQUAL(string("°C"), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
			CPPUNIT_ASSERT_EQUAL(3, it.values().size());
			CPPUNIT_ASSERT_EQUAL("zero", it.values().at(0));
			CPPUNIT_ASSERT_EQUAL("two", it.values().at(2));
			CPPUNIT_ASSERT_EQUAL("four", it.values().at(4));
		}
		else if (it.id() == TypeInfoID::parse("availability")) {
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT(!it.range().isValid());
			CPPUNIT_ASSERT_EQUAL(3, it.values().size());
			CPPUNIT_ASSERT_EQUAL("minus one", it.values().at(-1));
			CPPUNIT_ASSERT_EQUAL("minus two", it.values().at(-2));
			CPPUNIT_ASSERT_EQUAL("minus three", it.values().at(-3));
		}
		else {
			CPPUNIT_ASSERT(false);
		}
	}
}

void TypesSAXHandlerTest::testParseHasBothValuesAndRange()
{
	CPPUNIT_ASSERT_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"temperature\">"
			"    <values>"
			"      <value equals=\"0\">zero</value>"
			"      <value equals=\"1\">one</value>"
			"    </values>"
			"    <range />"
			"  </type>"
			"</types>"),
		XMLException
	);

	CPPUNIT_ASSERT_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"humidity\">"
			"    <range />"
			"    <values>"
			"      <value equals=\"0\">zero</value>"
			"      <value equals=\"1\">one</value>"
			"    </values>"
			"  </type>"
			"</types>"),
		XMLException
	);
}

void TypesSAXHandlerTest::testParseInvalidValues()
{
	CPPUNIT_ASSERT_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"temperature\">"
			"    <values>"
			"      <value equals=\"0\">zero</value>"
			"      <value equals=\"text\">one</value>"
			"    </values>"
			"  </type>"
			"</types>"),
		SyntaxException
	);

	CPPUNIT_ASSERT_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"enum\">"
			"    <values>"
			"      <value equals=\"0\">zero</value>"
			"      <value equals=\"0\">one</value>"
			"    </values>"
			"  </type>"
			"</types>"),
		XMLException
	);
}

void TypesSAXHandlerTest::testParseWithLevels()
{
	CPPUNIT_ASSERT_NO_THROW(
		m_parser.parseString(
			"<types>"
			"  <type id=\"temperature\">"
			"    <levels>"
			"      <level equals=\"0\">zero</level>"
			"      <level equals=\"1\">one</level>"
			"    </levels>"
			"  </type>"
			"  <type id=\"humidity\">"
			"    <unit>&#176;C</unit>"
			"    <levels>"
			"      <level min=\"0\" max=\"1\">0..1</level>"
			"      <level min=\"2\" max=\"3\">2..3</level>"
			"      <level equals=\"4\">4</level>"
			"    </levels>"
			"  </type>"
			"  <type id=\"pressure\">"
			"    <levels>"
			"      <level max=\"10\">?..10</level>"
			"      <level equals=\"11\">minus two</level>"
			"      <level min=\"12\">12..?</level>"
			"    </levels>"
			"  </type>"
			"</types>")
	);

	for (auto it : *m_handler) {
		if (it.id() == TypeInfoID::parse("temperature")) {
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT_EQUAL(2, it.levels().size());
		}
		else if (it.id() == TypeInfoID::parse("humidity")) {
			CPPUNIT_ASSERT_EQUAL(string("°C"), it.unit());
			CPPUNIT_ASSERT_EQUAL(3, it.levels().size());
		}
		else if (it.id() == TypeInfoID::parse("pressure")) {
			CPPUNIT_ASSERT_EQUAL(string(""), it.unit());
			CPPUNIT_ASSERT_EQUAL(3, it.levels().size());
		}
		else {
			CPPUNIT_ASSERT(false);
		}
	}
}

void TypesSAXHandlerTest::testParseOverlappingLevels()
{
	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type>"
		"    <levels>"
		"      <level equals=\"10\">10</level>"
		"      <level equals=\"10\">10</level>"
		"    </levels>"
		"  </type>"
		"</types>"),
		XMLException);

	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type>"
		"    <levels>"
		"      <level min=\"0\" max=\"10\">0..10</level>"
		"      <level min=\"10\" max=\"20\">10..20</level>"
		"    </levels>"
		"  </type>"
		"</types>"),
		XMLException);

	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type>"
		"    <levels>"
		"      <level equals=\"10\">10</level>"
		"      <level min=\"10\" max=\"20\">10..20</level>"
		"    </levels>"
		"  </type>"
		"</types>"),
		XMLException);

	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type>"
		"    <levels>"
		"      <level max=\"10\">10</level>"
		"      <level min=\"10\">10</level>"
		"    </levels>"
		"  </type>"
		"</types>"),
		XMLException);
}

void TypesSAXHandlerTest::testParseMissingId()
{
	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type>"
		"  </type>"
		"</types>"),
		XMLException);
}

void TypesSAXHandlerTest::testParseMultipleUnits()
{
	CPPUNIT_ASSERT_NO_THROW(m_parser.parseString(
		"<types>"
		"  <type id=\"pressure\">"
		"    <units>"
		"      <unit>g</unit>"
		"      <unit mul-by=\"1000\">kg</unit>"
		"      <unit div-by=\"1000\">mg</unit>"
		"    </units>"
		"  </type>"
		"</types>")
	);

	auto it = m_handler->begin();
	CPPUNIT_ASSERT(it != m_handler->end());
	CPPUNIT_ASSERT(TypeInfoID::parse("pressure") == it->id());
	CPPUNIT_ASSERT_EQUAL("g", it->unit());
}

void TypesSAXHandlerTest::testParseWrongFirstUnit()
{
	CPPUNIT_ASSERT_THROW(m_parser.parseString(
		"<types>"
		"  <type id=\"pressure\">"
		"    <units>"
		"      <unit mul-by=\"1\">g</unit>"
		"    </units>"
		"  </type>"
		"</types>"),
		InvalidArgumentException
	);
}

}
