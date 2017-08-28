#ifndef BEEEON_EXTENDED_WORK_EXECUTOR_H
#define BEEEON_EXTENDED_WORK_EXECUTOR_H

#include "work/WorkExecutor.h"
#include "work/WorkLockManager.h"
#include "work/WorkScheduler.h"

namespace BeeeOn {

/**
 * @brief Abstract WorkExecutor that allows to wakeup
 * and cancel Work instances.
 */
class ExtendedWorkExecutor : public WorkExecutor {
public:
	~ExtendedWorkExecutor();

	void setScheduler(WorkScheduler::Ptr scheduler);
	void setLockManager(WorkLockManager::Ptr lockManager);

protected:
	/**
	 * Get pointer to the WorkScheduler instance for internal purposes.
	 */
	WorkScheduler::Ptr scheduler() const;

	/**
	 * Get pointer to the WorkLockManager instance for internal purposes.
	 */
	WorkLockManager::Ptr lockManager() const;

private:
	WorkScheduler::Ptr m_scheduler;
	WorkLockManager::Ptr m_lockManager;
};

}

#endif
