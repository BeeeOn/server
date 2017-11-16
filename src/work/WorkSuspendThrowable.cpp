#include "work/WorkSuspendThrowable.h"

using namespace Poco;
using namespace BeeeOn;

WorkSuspendThrowable::WorkSuspendThrowable(const Timespan &duration):
	m_duration(duration)
{
}

Timespan WorkSuspendThrowable::duration() const
{
	return m_duration;
}

WorkSuspendForEventThrowable::WorkSuspendForEventThrowable()
{
}
