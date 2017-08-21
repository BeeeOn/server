#include <string>

#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "work/Work.h"

using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

Work::Work():
	m_state(STATE_IDLE),
	m_priority(0),
	m_sleepDuration(0),
	m_suspended(0),
	m_finished(0)
{
}

Work::Work(const ID &id):
	Entity(id),
	m_state(STATE_IDLE),
	m_priority(0),
	m_sleepDuration(0),
	m_suspended(0),
	m_finished(0)
{
}

Work::~Work()
{
}

void Work::setState(State state)
{
	m_state = state;
}

Work::State Work::state() const
{
	return m_state;
}

void Work::setPriority(int priority)
{
	m_priority = priority;
}

int Work::priority() const
{
	return m_priority;
}

Nullable<Timestamp> Work::activationTime() const
{
	Nullable<Timestamp> result;

	if (!m_sleepDuration.isNull()) {
		if (m_suspended.epochMicroseconds() > 0)
			result = m_suspended + m_sleepDuration.value();
		else
			result = m_created + m_sleepDuration.value();
	}

	return result;
}

void Work::setNoSleepDuration()
{
	m_sleepDuration.clear();
}

void Work::setSleepDuration(const Timespan &duration)
{
	if (duration < 0)
		throw InvalidArgumentException("duration must not be negative");

	m_sleepDuration = duration;
}

void Work::setSleepDuration(const Nullable<Timespan> &duration)
{
	if (duration.isNull())
		setNoSleepDuration();
	else
		setSleepDuration(duration.value());
}

Nullable<Timespan> Work::sleepDuration() const
{
	return m_sleepDuration;
}

void Work::setCreated(const Timestamp &created)
{
	m_created = created;
}

Timestamp Work::created() const
{
	return m_created;
}

void Work::setSuspended(const Timestamp &suspended)
{
	m_suspended = suspended;
}

Timestamp Work::suspended() const
{
	return m_suspended;
}

void Work::setFinished(const Timestamp &finished)
{
	m_finished = finished;
}

Timestamp Work::finished() const
{
	return m_finished;
}

void Work::setContent(const WorkContent &content)
{
	m_content = content;
}

WorkContent &Work::content()
{
	return m_content;
}

WorkContent Work::content() const
{
	return m_content;
}
