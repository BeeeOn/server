#ifndef BEEEON_GENERIC_WORK_RUNNER_H
#define BEEEON_GENERIC_WORK_RUNNER_H

#include "util/Loggable.h"
#include "work/WorkRunner.h"

namespace BeeeOn {

class WorkWriteGuard;

class GenericWorkRunner : public WorkRunner, Loggable {
public:
	GenericWorkRunner(WorkScheduler &scheduler, WorkLockManager &lockManager);
	~GenericWorkRunner();

	void run() override;

protected:
	virtual void prepare();
	virtual void execute();
	void doSuspend(WorkWriteGuard &guard);
	void doFinish();
	void doFailed();
};

class GenericWorkRunnerFactory : public WorkRunnerFactory {
public:
	WorkRunner *create(WorkScheduler &scheduler) override;
};

}

#endif
