#include <unicode/strenum.h>
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
	CPPUNIT_TEST(testSpecificID);
	CPPUNIT_TEST(testCETByOffset);
	CPPUNIT_TEST(testEETByOffset);
	CPPUNIT_TEST(testZuluByOffset);
	CPPUNIT_TEST(testUTCm06ByOffset);
	CPPUNIT_TEST_SUITE_END();

public:
	void testPrague();
	void testLondon();
	void testSpecificID();
	void testCETByOffset();
	void testEETByOffset();
	void testZuluByOffset();
	void testUTCm06ByOffset();

private:
	SharedPtr<icu::TimeZone> byID(const string &id);
	SharedPtr<icu::TimeZone> byOffset(const int32_t rawOffset);
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

/**
 * Test GMT+01:00 specific time zones. Thowse zones are low-level and the DST
 * does not applies to them.
 */
void IcuTimeZoneImplTest::testSpecificID()
{
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);

	const IcuTimeZoneImpl zone(byID("GMT+01:00"));

	CPPUNIT_ASSERT_EQUAL("GMT+01:00", zone.id());

	CPPUNIT_ASSERT_EQUAL(1, zone.utcOffset().hours());
	CPPUNIT_ASSERT_EQUAL(1, zone.dstOffset().hours());
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_06_24));
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_12_24));

	CPPUNIT_ASSERT_EQUAL(
		"GMT+01:00",
		zone.shortName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"GMT+01:00",
		zone.shortName(Locale::system(), T2017_06_24)
	);

	CPPUNIT_ASSERT_EQUAL(
		"GMT+01:00",
		zone.displayName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"GMT+01:00",
		zone.displayName(Locale::system(), T2017_06_24)
	);
}

SharedPtr<icu::TimeZone> IcuTimeZoneImplTest::byOffset(const int32_t rawOffset)
{
	UErrorCode error = U_ZERO_ERROR;

	SharedPtr<icu::StringEnumeration> e =
		icu::TimeZone::createTimeZoneIDEnumeration(
			UCAL_ZONE_TYPE_CANONICAL,
			"001", // world
			&rawOffset,
			error
		);

	CPPUNIT_ASSERT_MESSAGE(u_errorName(error), U_SUCCESS(error));

	const UnicodeString *id;

	UnicodeString unknownID;
	icu::TimeZone::getUnknown().getID(unknownID);

	while ((id = e->snext(error)) != NULL) {
		CPPUNIT_ASSERT_MESSAGE(u_errorName(error), U_SUCCESS(error));

		SharedPtr<icu::TimeZone> impl = icu::TimeZone::createTimeZone(*id);
		UnicodeString tmp;

		if (impl->getID(tmp) == unknownID)
			continue; // skip unknown ID, should not happen here

		return impl;
	}

	return nullptr;
}

void IcuTimeZoneImplTest::testCETByOffset()
{
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);

	IcuTimeZoneImpl zone(byOffset(1 * 60 * 60 * 1000));

	CPPUNIT_ASSERT_EQUAL("CET", zone.id());

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
		"CET (GMT+01:00)",
		zone.displayName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"CET (GMT+02:00)",
		zone.displayName(Locale::system(), T2017_06_24)
	);
}

void IcuTimeZoneImplTest::testEETByOffset()
{
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);

	IcuTimeZoneImpl zone(byOffset(2 * 60 * 60 * 1000));

	CPPUNIT_ASSERT_EQUAL("EET", zone.id());

	CPPUNIT_ASSERT_EQUAL(2, zone.utcOffset().hours());
	CPPUNIT_ASSERT_EQUAL(1, zone.dstOffset().hours());
	CPPUNIT_ASSERT(zone.appliesDST(T2017_06_24));
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_12_24));

	CPPUNIT_ASSERT_EQUAL(
		"GMT+02:00",
		zone.shortName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"GMT+03:00",
		zone.shortName(Locale::system(), T2017_06_24)
	);

	CPPUNIT_ASSERT_EQUAL(
		"EET (GMT+02:00)",
		zone.displayName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"EET (GMT+03:00)",
		zone.displayName(Locale::system(), T2017_06_24)
	);
}

void IcuTimeZoneImplTest::testZuluByOffset()
{
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);

	IcuTimeZoneImpl zone(byOffset(0));

	CPPUNIT_ASSERT_EQUAL("Etc/GMT", zone.id());

	CPPUNIT_ASSERT_EQUAL(0, zone.utcOffset().hours());
	CPPUNIT_ASSERT_EQUAL(0, zone.dstOffset().hours());
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_06_24));
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_12_24));

	CPPUNIT_ASSERT_EQUAL(
		"GMT",
		zone.shortName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"GMT",
		zone.shortName(Locale::system(), T2017_06_24)
	);

	const string name_12_24 = zone.displayName(Locale::system(), T2017_12_24);
	const string name_06_24 = zone.displayName(Locale::system(), T2017_06_24);

	CPPUNIT_ASSERT(
		"Greenwich Mean Time (GMT)" == name_12_24
		|| "Etc/GMT (GMT)" == name_12_24);
	CPPUNIT_ASSERT(
		"Greenwich Mean Time (GMT)" == name_06_24
		|| "Etc/GMT (GMT)" == name_06_24);
}

void IcuTimeZoneImplTest::testUTCm06ByOffset()
{
	static const Timestamp T2017_06_24 = Timestamp::fromEpochTime(1498255200);
	static const Timestamp T2017_12_24 = Timestamp::fromEpochTime(1514070000);

	IcuTimeZoneImpl zone(byOffset(-6 * 60 * 60 * 1000));

	CPPUNIT_ASSERT_EQUAL("CST6CDT", zone.id());

	CPPUNIT_ASSERT_EQUAL(-6, zone.utcOffset().hours());
	CPPUNIT_ASSERT_EQUAL(1, zone.dstOffset().hours());
	CPPUNIT_ASSERT(zone.appliesDST(T2017_06_24));
	CPPUNIT_ASSERT(!zone.appliesDST(T2017_12_24));

	CPPUNIT_ASSERT_EQUAL(
		"GMT-06:00",
		zone.shortName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"GMT-05:00",
		zone.shortName(Locale::system(), T2017_06_24)
	);

	CPPUNIT_ASSERT_EQUAL(
		"Central Standard Time (GMT-06:00)",
		zone.displayName(Locale::system(), T2017_12_24)
	);
	CPPUNIT_ASSERT_EQUAL(
		"Central Daylight Time (GMT-05:00)",
		zone.displayName(Locale::system(), T2017_06_24)
	);
}

}
