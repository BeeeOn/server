#ifndef BEEEON_BASIC_WORK_PROCESSOR_H
#define BEEEON_BASIC_WORK_PROCESSOR_H

#include <list>
#include <string>

#include <Poco/AtomicCounter.h>
#include <Poco/Runnable.h>
#include <Poco/Thread.h>
#include <Poco/ThreadPool.h>

#include "loop/StoppableRunnable.h"
#include "util/HavingThreadPool.h"
#include "work/BasicQueue.h"
#include "work/WorkScheduler.h"
#include "work/WorkLockManager.h"

namespace BeeeOn {

class WorkBackup;
class WorkExecutor;
class WorkRunner;
class WorkRunnerFactory;

class BasicProcessor :
		public WorkScheduler,
		public StoppableRunnable,
		public HavingThreadPool {
public:
	BasicProcessor();

	void setBackup(WorkBackup *repository);
	void setRunnerFactory(WorkRunnerFactory *factory);
	void setLockManager(WorkLockManager::Ptr manager);

	void run() override;
	void stop() override;

	void schedule(Work::Ptr work) override;
	void wakeup(Work::Ptr work) override;
	void cancel(Work::Ptr work) override;
	void notify() override;

	void registerExecutor(WorkExecutor *executor);

	void init();

protected:
	void initQueue();
	void dispatch();
	void execute(WorkExecutor *executor, Work::Ptr work);
	bool executeInThread(Poco::Runnable &runnable,
			const std::string &name);
	void wakeUpSelf();

private:
	WorkBackup *m_backup;
	WorkRunnerFactory *m_runnerFactory;
	WorkLockManager::Ptr m_lockManager;
	std::list<WorkExecutor *> m_executors;
	Poco::AtomicCounter m_shouldStop;
	Poco::Thread *m_current;
	BasicQueue m_queue;
};

}

#endif
