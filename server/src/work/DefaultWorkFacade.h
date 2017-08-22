#ifndef BEEEON_DEFAULT_WORK_FACADE_H
#define BEEEON_DEFAULT_WORK_FACADE_H

#include <map>

#include <Poco/RWLock.h>
#include <Poco/SharedPtr.h>

#include "policy/WorkAccessPolicy.h"
#include "work/Work.h"
#include "work/WorkFacade.h"

namespace BeeeOn {

class UserPolicyContext;
class WorkLockManager;
class WorkScheduler;

class DefaultWorkFacade : public WorkFacade {
public:
	DefaultWorkFacade();

	void schedule(Work &work, const PolicyContext &context) override;
	void wakeup(Work &work, const PolicyContext &context) override;
	void cancel(Work &work, const PolicyContext &context) override;
	bool fetch(Work &work, const PolicyContext &context) override;
	bool remove(const Work &work, const PolicyContext &context) override;

	void setScheduler(Poco::SharedPtr<WorkScheduler> scheduler);
	void setLockManager(Poco::SharedPtr<WorkLockManager> manager);
	void setAccessPolicy(WorkAccessPolicy::Ptr policy);

protected:
	void apply(Work::Ptr &work, const Work &data) const;
	const UserPolicyContext &asUserContext(const PolicyContext &context) const;

private:
	mutable Poco::SharedPtr<WorkLockManager> m_lockManager;
	Poco::SharedPtr<WorkScheduler> m_scheduler;
	WorkAccessPolicy::Ptr m_accessPolicy;
	std::map<WorkID, Work::Ptr> m_storage;
	Poco::RWLock m_lock;
};

}

#endif
