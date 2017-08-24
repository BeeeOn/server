#ifndef BEEEON_WORK_SCHEDULER_H
#define BEEEON_WORK_SCHEDULER_H

#include <Poco/SharedPtr.h>

#include "work/Work.h"

namespace BeeeOn {

class WorkScheduler {
public:
	typedef Poco::SharedPtr<WorkScheduler> Ptr;

	virtual ~WorkScheduler();

	/**
	 * Schedule the given work for execution.
	 */
	virtual void schedule(Work::Ptr work) = 0;

	/**
	 * Wake up the given work to be executed as soon as possible.
	 */
	virtual void wakeup(Work::Ptr work) = 0;

	/**
	 * Remove the given work from the scheduler.
	 * Do not schedule it again.
	 */
	virtual void cancel(Work::Ptr work) = 0;

	/**
	 * Notify scheduler to check whether something
	 * can be done.
	 */
	virtual void notify() = 0;
};

}

#endif
