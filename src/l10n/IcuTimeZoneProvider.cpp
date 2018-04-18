#include <unicode/strenum.h>
#include <unicode/timezone.h>
#include <unicode/ucal.h>
#include <unicode/unistr.h>

#include <Poco/Logger.h>
#include <Poco/SharedPtr.h>

#include "di/Injectable.h"
#include "l10n/IcuTimeZoneImpl.h"
#include "l10n/IcuTimeZoneProvider.h"
#include "l10n/TimeZone.h"

BEEEON_OBJECT_BEGIN(BeeeOn, IcuTimeZoneProvider)
BEEEON_OBJECT_CASTABLE(TimeZoneProvider)
BEEEON_OBJECT_END(BeeeOn, IcuTimeZoneProvider)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Nullable<BeeeOn::TimeZone> IcuTimeZoneProvider::findById(const string &id)
{
	icu::UnicodeString uniID(id.c_str(), id.size());
	SharedPtr<icu::TimeZone> impl = icu::TimeZone::createTimeZone(uniID);

	icu::UnicodeString tmp;
	icu::UnicodeString unknownID;
	icu::TimeZone::getUnknown().getID(unknownID);

	Nullable<TimeZone> zone;

	if (impl->getID(tmp) == unknownID)
		return zone;

	zone = TimeZone(new IcuTimeZoneImpl(impl));
	return zone;
}

Nullable<BeeeOn::TimeZone> IcuTimeZoneProvider::findByOffset(const Timespan &offset)
{
	Nullable<BeeeOn::TimeZone> zone;
	UErrorCode error = U_ZERO_ERROR;
	const int32_t rawOffset = offset.totalMilliseconds();

	SharedPtr<icu::StringEnumeration> e =
		icu::TimeZone::createTimeZoneIDEnumeration(
			UCAL_ZONE_TYPE_CANONICAL,
			"001", // world
			&rawOffset,
			error
		);

	if (!U_SUCCESS(error)) {
		logger().warning(u_errorName(error), __FILE__, __LINE__);
		return zone;
	}

	const icu::UnicodeString *id;

	icu::UnicodeString unknownID;
	icu::TimeZone::getUnknown().getID(unknownID);

	while ((id = e->snext(error)) != NULL) {
		if (!U_SUCCESS(error)) {
			logger().warning(u_errorName(error), __FILE__, __LINE__);
			continue; // skip
		}

		SharedPtr<icu::TimeZone> impl = icu::TimeZone::createTimeZone(*id);
		icu::UnicodeString tmp;
		if (impl->getID(tmp) == unknownID)
			continue; // skip unknown ID, should not happen here

		zone = TimeZone(new IcuTimeZoneImpl(impl));
		break;
	}

	return zone;
}

void IcuTimeZoneProvider::listTimeZones(vector<TimeZone> &zones)
{
	UErrorCode error = U_ZERO_ERROR;

	SharedPtr<icu::StringEnumeration> e =
		icu::TimeZone::createTimeZoneIDEnumeration(
			UCAL_ZONE_TYPE_CANONICAL_LOCATION,
			NULL,
			NULL,
			error
		);

	if (!U_SUCCESS(error)) {
		logger().warning(
				u_errorName(error)
				+ string(" fallback to createEnumeration()"),
				__FILE__, __LINE__);

		e = icu::TimeZone::createEnumeration();
	}

	const icu::UnicodeString *id;

	icu::UnicodeString unknownID;
	icu::TimeZone::getUnknown().getID(unknownID);

	while ((id = e->snext(error)) != NULL) {
		if (!U_SUCCESS(error)) {
			logger().warning(u_errorName(error), __FILE__, __LINE__);
			continue;
		}

		SharedPtr<icu::TimeZone> impl = icu::TimeZone::createTimeZone(*id);
		icu::UnicodeString tmp;

		if (impl->getID(tmp) == unknownID)
			continue; // skip unknown ID, should not happen here

		TimeZone zone(new IcuTimeZoneImpl(impl));
		zones.push_back(zone);
	}
}
