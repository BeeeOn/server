#include <cstring>

#include <cppunit/extensions/HelperMacros.h>

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>
#include <Poco/Timezone.h>

#include "cppunit/BetterAssert.h"
#include "l10n/TimeZoneImpl.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class TimeZoneImplTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(TimeZoneImplTest);
	CPPUNIT_TEST(testSystemTZ);
	CPPUNIT_TEST_SUITE_END();

public:
	void testSystemTZ();
};

CPPUNIT_TEST_SUITE_REGISTRATION(TimeZoneImplTest);

void TimeZoneImplTest::testSystemTZ()
{
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);

	const SharedPtr<TimeZoneImpl> tz = new SystemTimeZoneImpl;
	const string utcSign = tz->utcOffset() < 0? "-" : "+";

	CPPUNIT_ASSERT_EQUAL(Timezone::standardName(), tz->id());

	if (Timezone::isDst(T2017_12_24)) {
		CPPUNIT_ASSERT_EQUAL(
			Timezone::dstName(),
			tz->displayName(Locale::system(), T2017_12_24)
		);
	}
	else {
		CPPUNIT_ASSERT_EQUAL(
			Timezone::standardName(),
			tz->displayName(Locale::system(), T2017_12_24)
		);
	}

	CPPUNIT_ASSERT_EQUAL(
		utcSign,
		tz->shortName(Locale::system(), T2017_12_24).substr(0, 1)
	);

	if (Timezone::isDst(T2017_06_24)) {
		CPPUNIT_ASSERT_EQUAL(
			Timezone::dstName(),
			tz->displayName(Locale::system(), T2017_06_24)
		);
	}
	else {
		CPPUNIT_ASSERT_EQUAL(
			Timezone::standardName(),
			tz->displayName(Locale::system(), T2017_06_24)
		);
	}

	CPPUNIT_ASSERT_EQUAL(
		utcSign,
		tz->shortName(Locale::system(), T2017_06_24).substr(0, 1)
	);
}

}
