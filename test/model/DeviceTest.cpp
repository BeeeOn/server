#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "model/Device.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class DeviceTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DeviceTest);
	CPPUNIT_TEST(testSetRefresh);
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
	CPPUNIT_ASSERT_EQUAL(0, device.refresh().seconds());

	device.setRefresh(10);
	CPPUNIT_ASSERT_EQUAL(10, device.refresh().seconds());
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

	device.setRefresh(RefreshTime::NONE);
	CPPUNIT_ASSERT(!device.hasRefresh());
}

}
