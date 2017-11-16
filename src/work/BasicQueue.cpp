#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "work/BasicQueue.h"
#include "work/WorkBackup.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

void BasicQueue::assertLocked()
{
	if (!m_lock.tryLock())
		return;

	m_lock.unlock();
	throw IllegalStateException("lock is not held in the basic queue");
}

void BasicQueue::pushUnlocked(Work::Ptr work)
{
	assertLocked();

	const Work::State state = work->state();
	if (state >= Work::STATE_FINISHED) {
		if (logger().debug()) {
			logger().debug("ignore finished work " + *work,
					__FILE__, __LINE__);
		}

		return;
	}

	doPushUnfinishedUnlocked(work);
	m_wakeup.set();
}

void BasicQueue::activate(const Key &key, Record &record)
{
	if (logger().debug()) {
		logger().debug("activating work " + *record.work
				+ " priority " + to_string(key.priority)
				+ " at " + to_string(key.at.epochMicroseconds()),
				__FILE__, __LINE__);
	}

	record.ref = m_active.emplace(make_pair(key, record.work));
}

void BasicQueue::deactivate(Record &record)
{
	m_active.erase(record.ref);
	record.ref = m_active.end();
}

void BasicQueue::doPushUnfinishedUnlocked(Work::Ptr work)
{
	const Nullable<Timestamp> time = work->activationTime();
	const int priority = work->priority();
	const WorkID id = work->id();

	auto it = m_queue.find(id);

	if (it != m_queue.end()) {
		if (logger().debug())
			logger().debug("reschedule work " + *work, __FILE__, __LINE__);

		Record &record = it->second;


		// if the work is active and the activation time has
		// changed or is not specified anymore, remove that
		// work from the active queue
		if (record.ref != m_active.end()) {
			const Key &key = record.ref->first;

			if (time.isNull())
				deactivate(record);
			else if (time.value() != key.at)
				deactivate(record);
			else if (priority != key.priority)
				deactivate(record);
		}

		// if the work is not active (or was recently deactivated)
		// activate it if the activation time is specified
		if (record.ref == m_active.end()) {
			if (!time.isNull())
				activate(Key{time, priority}, record);
		}
	}
	else {
		if (logger().debug())
			logger().debug("new work " + *work, __FILE__, __LINE__);

		Record record = {work, m_active.end()};

		// activate if the activation time is specified
		if (!time.isNull())
			activate(Key{time, priority}, record);

		m_queue.emplace(make_pair(id, record));
	}

	work->setState(Work::STATE_SCHEDULED);
}

void BasicQueue::wakeupUnlocked(Work::Ptr work)
{
	assertLocked();

	if (m_queue.find(work->id()) == m_queue.end())
		return; // no such work, no reason to wake up

	work->setSleepDuration(0); // execute early
	pushUnlocked(work);

	m_wakeup.set();
}

void BasicQueue::cancelUnlocked(Work::Ptr work)
{
	assertLocked();

	const WorkID id = work->id();

	auto ref = m_queue.find(id);
	if (ref != m_queue.end()) {
		const Record &record = ref->second;
		m_active.erase(record.ref);
		m_queue.erase(ref);

		m_wakeup.set();
	}
}

pair<Timestamp, Work::Ptr> BasicQueue::popFast(const Timestamp &now)
{
	FastMutex::ScopedLock guard(m_lock);

	const auto head = m_active.begin();
	if (head == m_active.end()) // nothing to do
		return make_pair(now - 1, SharedPtr<Work>());

	const Key &key = head->first;
	if (key.at > now) // too early to execute
		return make_pair(key.at, SharedPtr<Work>());

	Work::Ptr work = head->second;
	const WorkID id = work->id();
	const Timestamp at(key.at);

	m_queue.erase(id);
	m_active.erase(head);

	return make_pair(at, work);
}

Work::Ptr BasicQueue::pop(bool blocking)
{
	bool shouldBlock = blocking;
	Timestamp now;

	do {
		now.update();

		pair<Timestamp, Work::Ptr> head = popFast(now);
		const Timestamp &at = head.first;
		Work::Ptr &work = head.second;

		if (at <= now && !work.isNull()) {
			if (logger().debug()) {
				logger().debug("pop work " + *work,
					__FILE__, __LINE__);
			}

			return work;
		}
		else if (shouldBlock) {
			shouldBlock = block(now, at);

			if (logger().debug()) {
				logger().debug("woken up, should block again: "
					+ to_string(shouldBlock),
					__FILE__, __LINE__);
			}
		}
		else
			return NULL;
	} while (1);
}

bool BasicQueue::block(
		const Poco::Timestamp &now,
		const Poco::Timestamp &next)
{
	if (next < now) {
		if (logger().debug()) {
			logger().debug("blocking until notified",
					__FILE__, __LINE__);
		}

		try {
			m_wakeup.wait();
			return false;
		}
		catch (const TimeoutException &e) {
			// no event, we should block more
			return true;
		}
	}
	else if (next > now) {
		const long us = next - now;
		const long sleep = (us / 1000) + ((us % 1000)? 1 : 0);

		if (logger().debug()) {
			logger().debug("blocking for "
					+ to_string(sleep)
					+ " ms",
					__FILE__, __LINE__);
		}

		return m_wakeup.tryWait(sleep);
	}

	return false; // probably, no reason to block
}

void BasicQueue::notify()
{
	m_wakeup.set();
}
