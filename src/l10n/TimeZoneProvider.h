#pragma once

#include <string>
#include <vector>

#include <Poco/Nullable.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "l10n/Locale.h"
#include "l10n/TimeZone.h"

namespace BeeeOn {

/**
 * @brief Provider that gives access methods to query available time zones.
 *
 * Every time zone library should have an implementation ofthe TimeZoneProvider. This
 * allows to access the time zones known for that library. There is usually only one
 * instance of a TimeZoneProvider in a running system.
 */
class TimeZoneProvider {
public:
	typedef Poco::SharedPtr<TimeZoneProvider> Ptr;

	virtual ~TimeZoneProvider();

	/**
	 * @returns time zone information by a specific ID.
	 * If no such time zone information is found, it returns NULL.
	 */
	virtual Poco::Nullable<TimeZone> findById(const std::string &id) = 0;

	/**
	 * @returns time zone information by the given UTC offset.
	 * If no such time zone information is found, it returns NULL.
	 */
	virtual Poco::Nullable<TimeZone> findByOffset(const Poco::Timespan &offset) = 0;

	/**
	 * @returns all supported time zone information in the system.
	 */
	virtual void listTimeZones(struct std::vector<TimeZone> &zones) = 0;
};

}
