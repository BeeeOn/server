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
	UnicodeString uniID(id.c_str(), id.size());
	SharedPtr<icu::TimeZone> impl = icu::TimeZone::createTimeZone(uniID);

	UnicodeString tmp;
	UnicodeString unknownID;
	icu::TimeZone::getUnknown().getID(unknownID);

	Nullable<TimeZone> zone;

	if (impl->getID(tmp) == unknownID)
		return zone;

	zone = TimeZone(new IcuTimeZoneImpl(impl));
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

	const UnicodeString *id;

	UnicodeString unknownID;
	icu::TimeZone::getUnknown().getID(unknownID);

	while ((id = e->snext(error)) != NULL) {
		if (!U_SUCCESS(error)) {
			logger().warning(u_errorName(error), __FILE__, __LINE__);
			continue;
		}

		SharedPtr<icu::TimeZone> impl = icu::TimeZone::createTimeZone(*id);
		UnicodeString tmp;

		if (impl->getID(tmp) == unknownID)
			continue; // skip unknown ID, should not happen here

		TimeZone zone(new IcuTimeZoneImpl(impl));
		zones.push_back(zone);
	}
}