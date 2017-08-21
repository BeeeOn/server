#ifndef BEEEON_WORK_EXECUTOR_H
#define BEEEON_WORK_EXECUTOR_H

#include "work/Work.h"

namespace Poco {

class Timespan;

}

namespace BeeeOn {

class WorkRunner;
class WorkScheduler;

class WorkExecutor {
public:
	virtual ~WorkExecutor();

	/**
	 * Tells whether the executor implementation can
	 * handle the given work.
	 */
	virtual bool accepts(const Work::Ptr work) const = 0;

	/**
	 * Execute the given work.
	 */
	virtual void execute(Work::Ptr work) = 0;

protected:
	void suspend(Work::Ptr work);
	void suspend(Work::Ptr work, const Poco::Timespan &duration);
};

class NullWorkExecutor : public WorkExecutor {
public:
	NullWorkExecutor();

	bool accepts(const Work::Ptr work) const override;
	void execute(Work::Ptr work) override;

	static WorkExecutor &instance();
};

}

#endif
