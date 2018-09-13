#pragma once

#include <string>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

#include "l10n/Locale.h"

namespace BeeeOn {

class TimeZoneImpl;

/**
 * @brief Representation of a time zone in the system.
 *
 * The TimeZone class is implementation-agnostic. It supports any time-zone
 * library via the TimeZoneImpl interface. This is the public API available
 * to the rest of the system.
 */
class TimeZone {
public:
	/**
	 * Constructs system time zone.
	 */
	TimeZone();

	template <typename T>
	TimeZone(Poco::SharedPtr<T> impl):
		m_impl(impl)
	{
		if (m_impl.isNull)
			throw Poco::BadCastException("failed to cast to TimeZoneImpl");
	}

	template <typename T>
	TimeZone(T *impl):
		m_impl(impl)
	{
	}

	/**
	 * @returns ID that is unique and fully identifies the time-zone for later reference.
	 */
	std::string id() const;

	/**
	 * @returns short display name of the time-zone for the given locale.
	 */
	std::string shortName(const Locale &locale, const Poco::Timestamp &at = {}) const;

	/**
	 * @returns display name of the time-zone for the given locale.
	 */
	std::string displayName(const Locale &locale, const Poco::Timestamp &at = {}) const;

	/**
	 * @returns UTC offset for computation of the time zone-based time.
	 */
	Poco::Timespan utcOffset() const;

	/**
	 * @returns date savings offset to be applied at certain time periods.
	 */
	Poco::Timespan dstOffset() const;

	/**
	 * @returns true when the given time is in the time period when date savings is to be applied.
	 */
	bool appliesDST(const Poco::Timestamp &at) const;

	Poco::SharedPtr<TimeZoneImpl> impl() const;

	static TimeZone system();

private:
	Poco::SharedPtr<TimeZoneImpl> m_impl;
};

}
