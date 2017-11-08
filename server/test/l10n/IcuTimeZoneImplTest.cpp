#include <unicode/timezone.h>
#include <unicode/unistr.h>

#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "l10n/IcuTimeZoneImpl.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class IcuTimeZoneImplTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(IcuTimeZoneImplTest);
	CPPUNIT_TEST(testPrague);
	CPPUNIT_TEST(testLondon);
	CPPUNIT_TEST_SUITE_END();

public:
	void testPrague();
	void testLondon();

private:
	SharedPtr<icu::TimeZone> byID(const string &id);
};

CPPUNIT_TEST_SUITE_REGISTRATION(IcuTimeZoneImplTest);

SharedPtr<icu::TimeZone> IcuTimeZoneImplTest::byID(const string &id)
{
	icu::UnicodeString unicodeID(id.c_str(), id.size());
	return icu::TimeZone::createTimeZone(unicodeID);
}

void IcuTimeZoneImplTest::testPrague()
{
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);

	const IcuTimeZoneImpl zone(byID("Europe/Prague"));

	CPPUNIT_ASSERT_EQUAL("Europe/Prague", zone.id());

	CPPUNIT_ASSERT_EQUAL(1, zone.utcOffset().hours());
	CPPUNIT_ASSERT_EQUAL(1, zone.dstOffset().hours());
	CPPUNIT_ASSERT(zone.appliesDST(T2017_06_24));
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_12_24));

	CPPUNIT_ASSERT_EQUAL(
		"GMT+01:00",
		zone.shortName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"GMT+02:00",
		zone.shortName(Locale::system(), T2017_06_24)
	);

	CPPUNIT_ASSERT_EQUAL(
		"Europe/Prague (GMT+01:00)",
		zone.displayName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"Europe/Prague (GMT+02:00)",
		zone.displayName(Locale::system(), T2017_06_24)
	);
}

void IcuTimeZoneImplTest::testLondon()
{
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);

	const IcuTimeZoneImpl zone(byID("Europe/London"));

	CPPUNIT_ASSERT_EQUAL("Europe/London", zone.id());

	CPPUNIT_ASSERT_EQUAL(0, zone.utcOffset().hours());
	CPPUNIT_ASSERT_EQUAL(1, zone.dstOffset().hours());
	CPPUNIT_ASSERT(zone.appliesDST(T2017_06_24));
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_12_24));

	CPPUNIT_ASSERT_EQUAL(
		"GMT",
		zone.shortName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"GMT+01:00",
		zone.shortName(Locale::system(), T2017_06_24)
	);

	CPPUNIT_ASSERT_EQUAL(
		"Europe/London (GMT)",
		zone.displayName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"Europe/London (GMT+01:00)",
		zone.displayName(Locale::system(), T2017_06_24)
	);
}

}
