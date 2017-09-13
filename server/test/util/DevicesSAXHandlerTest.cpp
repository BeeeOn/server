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
			"  <device id=\"0x01\" name=\"@NAME1\" vendor=\"@VENDOR1\">"
			"    <name>NAME1</name>"
			"    <manufacturer>VENDOR1</manufacturer>"
			"    <modules>"
			"      <sensor id=\"0x11\" type=\"0x01\">"
			"        <name>SENSOR1</name>"
			"      </sensor>"
			"    </modules>"
			"  </device>"
			"  <device id=\"0x02\" name=\"@NAME2\" vendor=\"@VENDOR2\">"
			"    <name>NAME2</name>"
			"    <manufacturer>VENDOR2</manufacturer>"
			"    <modules>"
			"      <sensor id=\"0x21\" type=\"0x02\">"
			"        <name>SENSOR2</name>"
			"      </sensor>"
			"      <refresh id=\"0x22\" type=\"0x0A\">"
			"        <default>10</default>"
			"      </refresh>"
			"      <actuator id=\"0x23\" type=\"0x04\">"
			"        <name>ACTUATOR1</name>"
			"      </actuator>"
			"    </modules>"
			"  </device>"
			"  <device id=\"0x03\" name=\"@NAME3\" vendor=\"@VENDOR3\">"
			"    <name>NAME3</name>"
			"    <manufacturer>VENDOR3</manufacturer>"
			"    <modules>"
			"      <sensor id=\"0x31\" type=\"0x03\" unavailable-value=\"0xffff\">"
			"        <name>SENSOR3</name>"
			"      </sensor>"
			"      <battery id=\"0x32\" type=\"0x08\" />"
			"      <rssi id=\"0x33\" type=\"0x09\" />"
			"      <actuator id=\"0x34\" type=\"0x02\">"
			"        <group>GROUP1</group>"
			"        <name>ACTUATOR2</name>"
			"      </actuator>"
			"    </modules>"
			"  </device>"
			"</devices>")
	);

	for (auto device : *m_handler) {
		if (device.id() == DeviceInfoID::parse("0x01")) {
			CPPUNIT_ASSERT_EQUAL(string("@NAME1"), device.name());
			CPPUNIT_ASSERT_EQUAL(string("NAME1"), device.displayName());
			CPPUNIT_ASSERT_EQUAL(string("@VENDOR1"), device.vendor());
			CPPUNIT_ASSERT_EQUAL(string("VENDOR1"), device.displayVendor());

			for (auto module : device) {
				if (module.id() == ModuleInfoID::parse("0x11")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(1), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("SENSOR1"), module.name());
					CPPUNIT_ASSERT(module.unavailable().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else {
					CPPUNIT_ASSERT(false);
				}
			}
		}
		else if (device.id() == DeviceInfoID::parse("0x02")) {
			CPPUNIT_ASSERT_EQUAL(string("@NAME2"), device.name());
			CPPUNIT_ASSERT_EQUAL(string("NAME2"), device.displayName());
			CPPUNIT_ASSERT_EQUAL(string("@VENDOR2"), device.vendor());
			CPPUNIT_ASSERT_EQUAL(string("VENDOR2"), device.displayVendor());

			for (auto module : device) {
				if (module.id() == ModuleInfoID::parse("0x21")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(2), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("SENSOR2"), module.name());
					CPPUNIT_ASSERT(module.unavailable().empty());
					CPPUNIT_ASSERT(module.defaultValue().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x22")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(10), module.type()->id());
					CPPUNIT_ASSERT(module.name().empty());
					CPPUNIT_ASSERT(module.unavailable().empty());
					CPPUNIT_ASSERT_EQUAL(string("10"), module.defaultValue());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x23")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(4), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("ACTUATOR1"), module.name());
					CPPUNIT_ASSERT(module.unavailable().empty());
					CPPUNIT_ASSERT(module.defaultValue().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else {
					CPPUNIT_ASSERT(false);
				}
			}
		}
		else if (device.id() == DeviceInfoID::parse("0x03")) {
			CPPUNIT_ASSERT_EQUAL(string("@NAME3"), device.name());
			CPPUNIT_ASSERT_EQUAL(string("NAME3"), device.displayName());
			CPPUNIT_ASSERT_EQUAL(string("@VENDOR3"), device.vendor());
			CPPUNIT_ASSERT_EQUAL(string("VENDOR3"), device.displayVendor());

			for (auto module : device) {
				if (module.id() == ModuleInfoID::parse("0x31")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(3), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("SENSOR3"), module.name());
					CPPUNIT_ASSERT_EQUAL(string("0xffff"), module.unavailable());
					CPPUNIT_ASSERT(module.defaultValue().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x32")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(8), module.type()->id());
					CPPUNIT_ASSERT(module.name().empty());
					CPPUNIT_ASSERT(module.unavailable().empty());
					CPPUNIT_ASSERT(module.defaultValue().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x33")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(9), module.type()->id());
					CPPUNIT_ASSERT(module.name().empty());
					CPPUNIT_ASSERT(module.unavailable().empty());
					CPPUNIT_ASSERT(module.defaultValue().empty());
					CPPUNIT_ASSERT(module.group().empty());
				}
				else if (module.id() == ModuleInfoID::parse("0x34")) {
					CPPUNIT_ASSERT_EQUAL(TypeInfoID(2), module.type()->id());
					CPPUNIT_ASSERT_EQUAL(string("ACTUATOR2"), module.name());
					CPPUNIT_ASSERT(module.unavailable().empty());
					CPPUNIT_ASSERT(module.defaultValue().empty());
					CPPUNIT_ASSERT_EQUAL(string("GROUP1"), module.group());
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
