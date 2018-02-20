#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "model/Device.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class DeviceTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DeviceTest);
	CPPUNIT_TEST(testSetRefresh);
	CPPUNIT_TEST(testSetRefreshNormalization);
	CPPUNIT_TEST(testHasRefresh);
	CPPUNIT_TEST_SUITE_END();

public:
	void testSetRefresh();
	void testSetRefreshNormalization();
	void testHasRefresh();
};

CPPUNIT_TEST_SUITE_REGISTRATION(DeviceTest);

void DeviceTest::testSetRefresh()
{
	Device device;

	device.setRefresh(0);
	CPPUNIT_ASSERT_EQUAL(0, device.refresh().totalMicroseconds());

	device.setRefresh(10);
	CPPUNIT_ASSERT_EQUAL(
		10 * Timespan::SECONDS,
		device.refresh().totalMicroseconds());

	device.setRefresh(Timespan(15, 1));
	CPPUNIT_ASSERT_EQUAL(
		15 * Timespan::SECONDS + 1,
		device.refresh().totalMicroseconds());
}

void DeviceTest::testSetRefreshNormalization()
{
	Device device;

	device.setRefresh(Timespan(1504, 0));
	CPPUNIT_ASSERT_EQUAL(
		1504 * Timespan::SECONDS,
		device.refresh().totalMicroseconds());

	device.setRefresh(Timespan(-1504, 0));
	CPPUNIT_ASSERT_EQUAL(
		-1,
		device.refresh().totalMicroseconds());

	device.setRefresh(Timespan(0, -1));
	CPPUNIT_ASSERT_EQUAL(
		-1,
		device.refresh().totalMicroseconds());

	device.setRefresh(-15);
	CPPUNIT_ASSERT_EQUAL(
		-1,
		device.refresh().totalMicroseconds());

	device.setRefresh(-1);
	CPPUNIT_ASSERT_EQUAL(
		-1,
		device.refresh().totalMicroseconds());
}

void DeviceTest::testHasRefresh()
{
	Device device;

	device.setRefresh(0);
	CPPUNIT_ASSERT(device.hasRefresh());

	device.setRefresh(10);
	CPPUNIT_ASSERT(device.hasRefresh());

	device.setRefresh(-1);
	CPPUNIT_ASSERT(!device.hasRefresh());

	device.setRefresh(Timespan(0, -1));
	CPPUNIT_ASSERT(!device.hasRefresh());
}

}
