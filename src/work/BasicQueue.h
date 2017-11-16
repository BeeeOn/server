#ifndef BEEEON_BASIC_QUEUE_H
#define BEEEON_BASIC_QUEUE_H

#include <map>

#include <Poco/Event.h>
#include <Poco/Mutex.h>
#include <Poco/Timestamp.h>

#include "util/Loggable.h"
#include "work/Work.h"

namespace BeeeOn {

class BasicQueue : Loggable {
public:
	/**
	 * Key used for ordering the queue for pop() operation.
	 */
	struct Key {
		Poco::Timestamp at;
		int priority;

		enum {
			EVERY_5SECS = 5
		};

		/**
		 * First, compare all works based on 5 seconds interval.
		 * If two works are in the same 5 seconds interval, then
		 * apply priority and finally the whole time in us.
		 */
		bool operator <(const Key &key) const
		{
			std::time_t period = at.epochTime() / EVERY_5SECS;

			if (period < key.at.epochTime() / EVERY_5SECS)
				return true;

			if (priority > key.priority) // higher is better
				return true;

			if (priority < key.priority) // higher is better
				return false;

			return at < key.at;
		}
	};

	/**
	 * Queue of active works. An active work must contain the
	 * activation time. Otherwise it must wait until wakeup.
	 */
	typedef std::multimap<Key, Work::Ptr> ActiveQueue;

	/**
	 * Record with queue information about a work.
	 */
	struct Record {
		Work::Ptr work;
		ActiveQueue::iterator ref;
	};

	/**
	 * Queue of all works waiting for execution.
	 * Some of them may just sleep until an event wakes them up.
	 */
	typedef std::map<WorkID, Record> WaitQueue;

	/**
	 * Provide the internal queue lock for callers because
	 * several operations might require custom locking connected
	 * tightly with the queue.
	 */
	Poco::FastMutex &lock()
	{
		return m_lock;
	}

	/**
	 * Push the given work into the queue.
	 * The BasicQueue::lock() must be held.
	 */
	void pushUnlocked(Work::Ptr work);

	/**
	 * Wakeup the given work if it is in the queue.
	 * The BasicQueue::lock() must be held.
	 */
	void wakeupUnlocked(Work::Ptr work);

	/**
	 * Cancel the given work if it is in the queue.
	 * The BasicQueue::lock() must be held.
	 *
	 * The Work state is not changed in any way by this call.
	 * You should likely call this method with work.executionLock()
	 * held. to prevent cancelling an already executed work.
	 */
	void cancelUnlocked(Work::Ptr work);

	/**
	 * Pop the head of the queue if any. The call can block
	 * until a work is available for processing. The blocking
	 * pop() can be interrupted by calling to notify(). It
	 * returns NULL in such case.
	 */
	Work::Ptr pop(bool blocking = true);

	/**
	 * Notify the queue that it should retry the blocking pop().
	 */
	void notify();

protected:
	void assertLocked();

	/**
	 * Do push with no Work access locking and no execution locking.
	 * If the given work is in STATE_EXECUTED, the Work::executionLock()
	 * must be held to prevent race conditions.
	 */
	void doPushUnfinishedUnlocked(Work::Ptr work);

	/**
	 * Pop the head of the queue if the Work is to be executed.
	 * Otherwise, it returns timestamp of the next Work. If
	 * the queue is empty, the returned timestamp is lower then now.
	 */
	std::pair<Poco::Timestamp, Work::Ptr> popFast(
			const Poco::Timestamp &now);

	/**
	 * Block for next - now microseconds or block indefinitely
	 * when now > next.
	 *
	 * @return true if blocking should be restarted when there is
	 *    still no work to be returned.
	 * @return false if blocking should be avoided due to notify()
	 *    has been delivered.
	 */
	bool block(const Poco::Timestamp &now, const Poco::Timestamp &next);

protected:
	void activate(const Key &key, Record &work);
	void deactivate(Record &record);

private:
	Poco::FastMutex m_lock;
	WaitQueue m_queue;
	ActiveQueue m_active;
	Poco::Event m_wakeup;
};

}

#endif
