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

string IcuTimeZoneImpl::worldName(const string &id, const Locale &locale, const Timestamp &at) const
{
	SharedPtr<IcuLocaleImpl> icuLocale = locale.impl().cast<IcuLocaleImpl>();

	char regionBuffer[4];
	UErrorCode error = U_ZERO_ERROR;
	const icu::UnicodeString utcId(id.c_str(), id.size());

	const int32_t regionLength = icu::TimeZone::getRegion(
			utcId, regionBuffer, sizeof(regionBuffer), error);

	if (!U_SUCCESS(error))
		return "";

	const string region(regionBuffer, regionLength);

	if (region == "001") {
		icu::UnicodeString uniName;
		if (icuLocale.isNull())
			m_zone->getDisplayName(appliesDST(at), icu::TimeZone::LONG, uniName);
		else
			m_zone->getDisplayName(appliesDST(at), icu::TimeZone::LONG, icuLocale->icu(), uniName);

		string name;
		return uniName.toUTF8String(name);
	}

	return "";
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

	const string &zoneId = id();
	const string &name = worldName(zoneId, locale, at);

	if (!name.empty() && name != utf8gmt)
		return name + " (" + utf8gmt + ")";

	if (utf8gmt != zoneId)
		return zoneId + " (" + utf8gmt + ")";

	return utf8gmt;
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
