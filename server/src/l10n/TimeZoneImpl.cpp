#include <Poco/Timezone.h>
#include <Poco/DateTimeFormatter.h>

#include "l10n/TimeZoneImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TimeZoneImpl::~TimeZoneImpl()
{
}

string TimeZoneImpl::shortName(const Locale &locale, const Timestamp &at) const
{
	const int utcOffset = Timezone::utcOffset();
	const Timespan absOffset =
		(utcOffset < 0 ? -utcOffset : utcOffset) * Timespan::SECONDS;

	if (absOffset < 0)
		return DateTimeFormatter::format(absOffset, "-%H:%M");
	else
		return DateTimeFormatter::format(absOffset, "+%H:%M");
}

string TimeZoneImpl::displayName(const Locale &locale, const Timestamp &at) const
{
	return id();
}

string SystemTimeZoneImpl::id() const
{
	return Timezone::standardName();
}

string SystemTimeZoneImpl::displayName(const Locale &locale, const Timestamp &at) const
{
	if (Timezone::isDst(at))
		return Timezone::dstName();
	else
		return Timezone::standardName();
}

Timespan SystemTimeZoneImpl::utcOffset() const
{
	return Timezone::utcOffset() * Timespan::SECONDS;
}

Timespan SystemTimeZoneImpl::dstOffset() const
{
	return Timezone::dst() * Timespan::SECONDS;
}

bool SystemTimeZoneImpl::appliesDST(const Timestamp &at) const
{
	return Timezone::isDst(at);
}
