#ifndef BEEEON_DEFAULT_WORK_FACADE_H
#define BEEEON_DEFAULT_WORK_FACADE_H

#include <map>

#include <Poco/RWLock.h>
#include <Poco/SharedPtr.h>

#include "work/Work.h"
#include "work/WorkFacade.h"

namespace BeeeOn {

class WorkLockManager;
class WorkScheduler;

class DefaultWorkFacade : public WorkFacade {
public:
	DefaultWorkFacade();

	void schedule(Work &work) override;
	void wakeup(Work &work) override;
	void cancel(Work &work) override;
	bool fetch(Work &work) override;
	bool remove(const Work &work) override;

	void setScheduler(Poco::SharedPtr<WorkScheduler> scheduler);
	void setLockManager(Poco::SharedPtr<WorkLockManager> manager);

protected:
	void apply(Work::Ptr &work, const Work &data) const;

private:
	mutable Poco::SharedPtr<WorkLockManager> m_lockManager;
	Poco::SharedPtr<WorkScheduler> m_scheduler;
	std::map<WorkID, Work::Ptr> m_storage;
	Poco::RWLock m_lock;
};

}

#endif
