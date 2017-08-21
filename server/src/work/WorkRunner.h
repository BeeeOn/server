#ifndef BEEEON_WORK_RUNNER_H
#define BEEEON_WORK_RUNNER_H

#include <Poco/Runnable.h>

#include "work/Work.h"

namespace BeeeOn {

class WorkBackup;
class WorkExecutor;
class WorkLockManager;
class WorkScheduler;

class WorkRunner : public Poco::Runnable {
public:
	WorkRunner(WorkScheduler &scheduler, WorkLockManager &lockManager);
	virtual ~WorkRunner();

	void setExecutor(WorkExecutor *executor);
	void setWork(Work::Ptr work);
	void setBackup(WorkBackup *backup);

protected:
	virtual void destroySelf();

protected:
	WorkScheduler &m_scheduler;
	WorkExecutor *m_executor;
	WorkBackup *m_backup;
	WorkLockManager &m_lockManager;
	Work::Ptr m_work;
};

class WorkRunnerFactory {
public:
	virtual ~WorkRunnerFactory();

	/**
	 * Create new WorkRunner for the given scheduler.
	 */
	virtual WorkRunner *create(
			WorkScheduler &scheduler) = 0;

	void setLockManager(WorkLockManager *manager);

protected:
	WorkLockManager *m_lockManager;
};

class NullWorkRunnerFactory : public WorkRunnerFactory {
public:
	WorkRunner *create(WorkScheduler &scheduler) override;

	static WorkRunnerFactory &instance();
};

}

#endif
