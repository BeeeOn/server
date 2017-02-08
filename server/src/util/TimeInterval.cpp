#include <string>

#include <Poco/Exception.h>

#include "util/TimeInterval.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TimeInterval::TimeInterval(
		const Timestamp &start, const Timestamp &end):
	m_start(start),
	m_end(end)
{
	if (m_start > m_end) {
		throw InvalidArgumentException("start ("
			+ to_string(m_start.epochTime())
			+ ") is after end ("
			+ to_string(m_end.epochTime())
			+ ")");
	}
}

TimeInterval::~TimeInterval()
{
}

bool TimeInterval::isEmpty() const
{
	return m_start == m_end;
}

bool TimeInterval::isBefore(const Timestamp &at) const
{
	return m_end <= at;
}

Timestamp TimeInterval::start() const
{
	return m_start;
}

Timestamp TimeInterval::end() const
{
	return m_end;
}

const Timestamp &TimeInterval::start()
{
	return m_start;
}

const Timestamp &TimeInterval::end()
{
	return m_end;
}
