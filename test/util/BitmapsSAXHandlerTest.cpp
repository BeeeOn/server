#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>
#include <Poco/XML/XMLException.h>
#include <Poco/SAX/SAXParser.h>

#include "cppunit/BetterAssert.h"
#include "util/BitmapsSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;

namespace BeeeOn {

class BitmapsSAXHandlerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(BitmapsSAXHandlerTest);
	CPPUNIT_TEST(testParseSimple);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testParseSimple();

private:
	SAXParser m_parser;
	SharedPtr<BitmapsSAXHandler> m_handler;
};

CPPUNIT_TEST_SUITE_REGISTRATION(BitmapsSAXHandlerTest);

void BitmapsSAXHandlerTest::setUp()
{
	m_handler = new BitmapsSAXHandler;
	m_parser.setContentHandler(m_handler);
}

void BitmapsSAXHandlerTest::tearDown()
{
	m_parser.setContentHandler(NULL);
}

void BitmapsSAXHandlerTest::testParseSimple()
{
	CPPUNIT_ASSERT_NO_THROW(
		m_parser.parseString(
			"<bitmaps>"
			"  <bitmap id=\"EMPTY\">"
			"  </bitmap>"
			"  <bitmap id=\"ERROR_FLAGS\">"
			"    <flags>"
			"      <flag index=\"0\">failed</flag>"
			"      <flag index=\"5\">running</flag>"
			"      <flag index=\"2\">fatal</flag>"
			"    </flags>"
			"  </bitmap>"
			"  <bitmap id=\"BCD_NUMBERS\">"
			"    <group label=\"index\">"
			"      <bit index=\"0\" />"
			"      <bit index=\"1\" />"
			"      <bit index=\"2\" />"
			"      <bit index=\"3\" />"
			"    </group>"
			"    <group label=\"value\">"
			"      <bit index=\"6\" />"
			"      <bit index=\"7\" />"
			"      <bit index=\"9\" />"
			"    </group>"
			"  </bitmap>"
			"</bitmaps>")
	);

	for (auto subtype : *m_handler) {
		const BitmapInfo &bitmap = subtype.bitmapInfo();

		if (subtype.id() == SubtypeInfoID::parse("EMPTY")) {
			CPPUNIT_ASSERT(bitmap.bits().empty());
			CPPUNIT_ASSERT(bitmap.groups().empty());
			CPPUNIT_ASSERT(bitmap.flags().empty());
		}
		else if (subtype.id() == SubtypeInfoID::parse("ERROR_FLAGS")) {
			CPPUNIT_ASSERT_EQUAL(3, bitmap.flags().size());
			CPPUNIT_ASSERT_EQUAL(0, bitmap.groups().size());
			CPPUNIT_ASSERT_EQUAL(3, bitmap.bits().size());

			CPPUNIT_ASSERT(bitmap.flags().find(0) != bitmap.flags().end());
			CPPUNIT_ASSERT(bitmap.flags().find(5) != bitmap.flags().end());
			CPPUNIT_ASSERT(bitmap.flags().find(2) != bitmap.flags().end());
		}
		else if (subtype.id() == SubtypeInfoID::parse("BCD_NUMBERS")) {
			CPPUNIT_ASSERT_EQUAL(0, bitmap.flags().size());
			CPPUNIT_ASSERT_EQUAL(2, bitmap.groups().size());
			CPPUNIT_ASSERT_EQUAL(2, bitmap.bits().size());

			auto it = bitmap.groups().begin();
			CPPUNIT_ASSERT(it != bitmap.groups().end());

			CPPUNIT_ASSERT_EQUAL(4, it->bits().size());

			++it;
			CPPUNIT_ASSERT(it != bitmap.groups().end());

			CPPUNIT_ASSERT_EQUAL(3, it->bits().size());
		}
		else {
			CPPUNIT_ASSERT(false);
		}
	}
}

}
