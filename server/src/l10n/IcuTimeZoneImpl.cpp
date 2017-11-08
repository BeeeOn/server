#include <unicode/calendar.h>
#include <unicode/timezone.h>
#include <unicode/unistr.h>
#include <unicode/utypes.h>

#include <Poco/Exception.h>

#include "l10n/IcuLocaleImpl.h"
#include "l10n/IcuTimeZoneImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

IcuTimeZoneImpl::IcuTimeZoneImpl(SharedPtr<icu::TimeZone> zone):
	m_zone(zone)
{
	if (m_zone.isNull())
		throw NullPointerException("icu zone is NULL");
}

string IcuTimeZoneImpl::id() const
{
	icu::UnicodeString result;
	m_zone->getID(result);

	string utf8;
	return result.toUTF8String(utf8);
}

string IcuTimeZoneImpl::shortName(const Locale &locale, const Timestamp &at) const
{
	SharedPtr<IcuLocaleImpl> icuLocale = locale.impl().cast<IcuLocaleImpl>();

	icu::UnicodeString gmt;

	if (icuLocale.isNull())
		m_zone->getDisplayName(appliesDST(at), icu::TimeZone::LONG_GMT, gmt);
	else
		m_zone->getDisplayName(appliesDST(at), icu::TimeZone::LONG_GMT, icuLocale->icu(), gmt);

	string utf8gmt;
	return gmt.toUTF8String(utf8gmt);
}

string IcuTimeZoneImpl::displayName(const Locale &locale, const Timestamp &at) const
{
	SharedPtr<IcuLocaleImpl> icuLocale = locale.impl().cast<IcuLocaleImpl>();

	icu::UnicodeString gmt;

	if (icuLocale.isNull())
		m_zone->getDisplayName(appliesDST(at), icu::TimeZone::LONG_GMT, gmt);
	else
		m_zone->getDisplayName(appliesDST(at), icu::TimeZone::LONG_GMT, icuLocale->icu(), gmt);

	string utf8gmt;
	gmt.toUTF8String(utf8gmt);

	return id() + " (" + utf8gmt + ")";
}

Timespan IcuTimeZoneImpl::utcOffset() const
{
	return m_zone->getRawOffset() * Timespan::MILLISECONDS;
}

Timespan IcuTimeZoneImpl::dstOffset() const
{
	return m_zone->getDSTSavings() * Timespan::MILLISECONDS;
}

bool IcuTimeZoneImpl::appliesDST(const Timestamp &at) const
{
	UErrorCode error = U_ZERO_ERROR;

	SharedPtr<icu::Calendar> calendar = icu::Calendar::createInstance(*m_zone, error);
	if (calendar.isNull()) {
		if (error != U_ZERO_ERROR)
			throw IllegalStateException(u_errorName(error));

		throw IllegalStateException("failed to create calendar");
	}

	calendar->setTime(at.epochMicroseconds() / 1000.0, error);
	if (!U_SUCCESS(error))
		throw IllegalStateException(u_errorName(error));

	const bool result = calendar->inDaylightTime(error);
	if (!U_SUCCESS(error))
		throw IllegalStateException(u_errorName(error));

	return result;
}
