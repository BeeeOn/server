#pragma once

#include <unicode/uversion.h>

#include <Poco/SharedPtr.h>

#include "l10n/TimeZoneImpl.h"

U_NAMESPACE_BEGIN

class TimeZone;

U_NAMESPACE_END

namespace BeeeOn {

/**
 * @brief ICU library-specific representation of a time zone.
 */
class IcuTimeZoneImpl : public TimeZoneImpl {
public:
	IcuTimeZoneImpl(Poco::SharedPtr<icu::TimeZone> zone);

	/**
	 * Gives ID as given by icu::TimeZone::getID().
	 */
	std::string id() const override;

	/**
	 * Gives short displat name like:
	 * <pre>
	 * GMT-08:00
	 * GMT+01:00
	 * </pre>
	 */
	std::string shortName(const Locale &locale,
			const Poco::Timestamp &at) const override;

	/**
	 * Give display name in format NAME (UTC_OFFSET).
	 * Example:
	 * <pre>
	 * America/Vancouver (GMT-08:00)
	 * Europe/Sarajevo (GMT+01:00)
	 * Pacific/Norfolk (GMT+11:00)
	 * </pre>
	 */
	std::string displayName(const Locale &locale,
			const Poco::Timestamp &at) const override;

	/**
	 * @return UTC offset with milliseconds resolution.
	 */
	Poco::Timespan utcOffset() const override;

	/**
	 * @return Daylight Savings offset with milliseconds resolution.
	 */
	Poco::Timespan dstOffset() const override;

	bool appliesDST(const Poco::Timestamp &at) const override;

protected:
	/**
	 * If the time zone with the given id is from region "001" (world),
	 * try to derive its display name. This allows to give names like
	 * <code>Central Standard Time</code>, <code>Greenwich Mean Time</code>,
	 * etc. The method returns an empty string from non-world time zones.
	 * For some time zones, the result is the LONG_GMT format like <code>GMT+1</code>.
	 */
	std::string worldName(const std::string &id,
			const Locale &locale,
			const Poco::Timestamp &at) const;

private:
	Poco::SharedPtr<icu::TimeZone> m_zone;
};

}
