#ifndef BEEEON_TIME_ZONE_IMPL_H
#define BEEEON_TIME_ZONE_IMPL_H

#include <string>

#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

#include "l10n/Locale.h"

namespace BeeeOn {

/**
 * @brief Interface for a certain time-zone system to be used.
 *
 * The TimeZoneImpl interface class allows to support many time-zone libraries
 * without affecting the rest of the system.
 */
class TimeZoneImpl {
public:
	virtual ~TimeZoneImpl();

	/**
	 * Return a unique ID of the time zone.
	 */
	virtual std::string id() const = 0;

	/**
	 * Provide a short name useful for displaying to end-users.
	 */
	virtual std::string shortName(const Locale &locale,
			const Poco::Timestamp &at) const;

	/**
	 * Provide a name useful for displaying to end-users.
	 */
	virtual std::string displayName(const Locale &locale,
			const Poco::Timestamp &at) const;

	/**
	 * Get the UTC offset without any DST savings.
	 */
	virtual Poco::Timespan utcOffset() const = 0;

	/**
	 * Return the DST offset if its to be applied for this timezone
	 * at a time. It is usualy 1 hour or nothing.
	 */
	virtual Poco::Timespan dstOffset() const = 0;

	/**
	 * Tell whether the DST is to be applied at the given time point.
	 */
	virtual bool appliesDST(const Poco::Timestamp &at) const = 0;
};

class SystemTimeZoneImpl : public TimeZoneImpl {
public:
	std::string id() const override;
	std::string displayName(const Locale &locale,
			const Poco::Timestamp &at) const override;
	Poco::Timespan utcOffset() const override;
	Poco::Timespan dstOffset() const override;
	bool appliesDST(const Poco::Timestamp &at) const override;
};

}

#endif
