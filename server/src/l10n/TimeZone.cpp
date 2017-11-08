#include "l10n/TimeZone.h"
#include "l10n/TimeZoneImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TimeZone::TimeZone():
	m_impl(new SystemTimeZoneImpl)
{
}

string TimeZone::id() const
{
	return m_impl->id();
}

string TimeZone::shortName(const Locale &locale, const Timestamp &at) const
{
	return m_impl->shortName(locale, at);
}

string TimeZone::displayName(const Locale &locale, const Timestamp &at) const
{
	return m_impl->displayName(locale, at);
}

Timespan TimeZone::utcOffset() const
{
	return m_impl->utcOffset();
}

Timespan TimeZone::dstOffset() const
{
	return m_impl->dstOffset();
}

bool TimeZone::appliesDST(const Timestamp &at) const
{
	return m_impl->appliesDST(at);
}

TimeZone TimeZone::system()
{
	return TimeZone(new SystemTimeZoneImpl);
}
