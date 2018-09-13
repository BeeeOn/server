#pragma once

#include "l10n/TimeZoneProvider.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief TimeZoneProvider implementation for the ICU library.
 */
class IcuTimeZoneProvider : public TimeZoneProvider, Loggable {
public:
	/**
	 * Find time zone information via ID as given by icu::TimeZone::getID().
	 */
	Poco::Nullable<TimeZone> findById(const std::string &id) override;

	/**
	 * Finds all compatible zones via icu::TimeZone::createTimeZoneIDEnumeration()
	 * with UCAL_ZONE_TYPE_CANONICAL parameter.
	 */
	Poco::Nullable<TimeZone> findByOffset(const Poco::Timespan &offset) override;

	/**
	 * List all time zones via icu::TimeZone::createTimeZoneIDEnumeration()
	 * with UCAL_ZONE_TYPE_CANONICAL_LOCATION parameter. This should provide
	 * only timezones related to a location like Europe/Prague, Pacific/Tahiti, etc.
	 *
	 * In case of a failure from createTimeZoneIDEnumeration() it fallbacks
	 * to calling icu::TimeZone::createEnumeration().
	 */
	void listTimeZones(struct std::vector<TimeZone> &zones) override;
};

}
