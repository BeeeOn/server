#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

#include <Poco/Exception.h>
#include <Poco/XML/XMLException.h>
#include <Poco/SAX/SAXParser.h>

#include "cppunit/BetterAssert.h"
#include "util/DevicesSAXHandler.h"

using namespace std;
using namespace Poco;
using namespace Poco::XML;

namespace BeeeOn {

class DevicesSAXHandlerTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DevicesSAXHandlerTest);
	CPPUNIT_TEST(testParseSimple);
	CPPUNIT_TEST_SUITE_END();
public:
	void setUp();
	void tearDown();

	void testParseSimple();

private:
	SAXParser m_parser;
	DevicesSAXHandler *m_handler;
};

CPPUNIT_TEST_SUITE_REGISTRATION(DevicesSAXHandlerTest);

void DevicesSAXHandlerTest::setUp()
{
	m_handler = new DevicesSAXHandler();
	m_parser.setContentHandler(m_handler);
}

void DevicesSAXHandlerTest::tearDown()
{
	m_parser.setContentHandler(NULL);
	delete m_handler;
}

void DevicesSAXHandlerTest::testParseSimple()
{
	CPPUNIT_ASSERT_NO_THROW(
		m_parser.parseString(
			"<devices>"
			"  <device id=\"0x01\">"
			"    <name>NAME1</name>"
			"    <vendor>VENDOR1</vendor>"
			"    <match>"
			"        <exact name=\"@NAME1\" vendor=\"@VENDOR1\" />"
			"    </match>"
			"    <modules>"
			"      <sensor id=\"0x11\" type=\"pressure\">"
			"        <name>SENSOR1</name>"
			"      </sensor>"
			"    </modules>"
			"  </device>"
			"  <device id=\"0x02\">"
			"    <name>NAME2</name>"
			"    <vendor>VENDOR2</vendor>"
			"    <match>"
			"        <exact name=\"@NAME2\" vendor=\"@VENDOR2\" />"
			"    </match>"
			"    <modules>"
			"      <sensor id=\"0x21\" type=\"humidity\">"
			"        <name>SENSOR2</name>"
			"      </sensor>"
			"      <refresh id=\"0x22\" type=\"luminance\">"
			"      </refresh>"
			"      <actuator id=\"0x23\" type=\"on_off\">"
			"        <name>ACTUATOR1</name>"
			"      </actuator>"
			"    </modules>"
			"  </device>"
			"  <device id=\"0x03\">"
			"    <name>NAME3</name>"
			"    <vendor>VENDOR3</vendor>"
			"    <match>"
			"        <pattern name=\"@*\" vendor=\"@VENDOR3\" />"
			"    </match>"
			"    <modules>"
			"      <sensor id=\"0x31\" type=\"temperature\">"
			"        <name>SENSOR3</name>"
			"        <unknown from=\"${raw} + 10 * 11\" />"
			"      </sensor>"
			"      <battery id=\"0x32\" type=\"battery\" />"
			"      <rssi id=\"0x33\" type=\"rssi\" />"
			"      <actuator id=\"0x34\" type=\"on_off\">"
			"        <group>GROUP1</group>"
			"        <name>ACTUATOR2</name>"
			"        <unknown from=\"${raw} + 1\" to=\"${value} - 1\" />"
			"      </actuator>"
			"    </modules>"
			"  </device>"
			"</devices>")
	);

	for (auto device : *m_handler) {
		if (device.id() == DeviceInfoID::parse("0x01")) {
			CPPUNIT_ASSERT(device.match("@NAME1", "@VENDOR1"));
			CPPUNIT_ASSERT_EQUAL(string("NAME1"), device.name());
			CPPUNIT_ASSERT_EQUAL(string("VENDOR1"), device.vendor());

			for (auto module : device) {
				if (module.id() == ModuleInfoID::parse("0x11")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("pressure"), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("SENSOR1"), module.name());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else {
					CPPUNIT_ASSERT(false);
				}
			}
		}
		else if (device.id() == DeviceInfoID::parse("0x02")) {
			CPPUNIT_ASSERT(device.match("@NAME2", "@VENDOR2"));
			CPPUNIT_ASSERT_EQUAL(string("NAME2"), device.name());
			CPPUNIT_ASSERT_EQUAL(string("VENDOR2"), device.vendor());

			for (auto module : device) {
				if (module.id() == ModuleInfoID::parse("0x21")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("humidity"), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("SENSOR2"), module.name());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x22")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("luminance"), module.type()->id());
					CPPUNIT_ASSERT(module.name().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x23")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("on_off"), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("ACTUATOR1"), module.name());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else {
					CPPUNIT_ASSERT(false);
				}
			}
		}
		else if (device.id() == DeviceInfoID::parse("0x03")) {
			CPPUNIT_ASSERT(device.match("@NAME3", "@VENDOR3"));
			CPPUNIT_ASSERT(device.match("@NAME-any", "@VENDOR3"));
			CPPUNIT_ASSERT(device.match("@any", "@VENDOR3"));
			CPPUNIT_ASSERT(!device.match("NAME3", "@VENDOR3"));
			CPPUNIT_ASSERT_EQUAL(string("NAME3"), device.name());
			CPPUNIT_ASSERT_EQUAL(string("VENDOR3"), device.vendor());

			for (auto module : device) {
				if (module.id() == ModuleInfoID::parse("0x31")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("temperature"), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("SENSOR3"), module.name());
					CPPUNIT_ASSERT(module.group().empty());
					CPPUNIT_ASSERT_EQUAL(string("${raw} + 10 * 11"), module.fromUnknown());
					CPPUNIT_ASSERT(module.toUnknown().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x32")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("battery"), module.type()->id());
					CPPUNIT_ASSERT(module.name().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x33")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("rssi"), module.type()->id());
					CPPUNIT_ASSERT(module.name().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x34")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID::parse("on_off"), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("ACTUATOR2"), module.name());
					CPPUNIT_ASSERT_EQUAL(string("GROUP1"), module.group());
					CPPUNIT_ASSERT_EQUAL(string("${raw} + 1"), module.fromUnknown());
					CPPUNIT_ASSERT_EQUAL(string("${value} - 1"), module.toUnknown());
				}
				else {
					CPPUNIT_ASSERT(false);
				}
			}
		}
		else {
			CPPUNIT_ASSERT(false);
		}
	}
}

}
