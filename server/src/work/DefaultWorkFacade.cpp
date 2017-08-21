#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "work/DefaultWorkFacade.h"
#include "work/WorkLockManager.h"
#include "work/WorkScheduler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DefaultWorkFacade)
BEEEON_OBJECT_CASTABLE(WorkFacade)
BEEEON_OBJECT_REF("scheduler", &DefaultWorkFacade::setScheduler)
BEEEON_OBJECT_REF("lockManager", &DefaultWorkFacade::setLockManager)
BEEEON_OBJECT_END(BeeeOn, DefaultWorkFacade)

using namespace Poco;
using namespace BeeeOn;

DefaultWorkFacade::DefaultWorkFacade()
{
}

void DefaultWorkFacade::setScheduler(SharedPtr<WorkScheduler> scheduler)
{
	m_scheduler = scheduler;
}

void DefaultWorkFacade::setLockManager(SharedPtr<WorkLockManager> manager)
{
	m_lockManager = manager;
}

void DefaultWorkFacade::apply(Work::Ptr &work, const Work &data) const
{
	WorkWriteGuard guard(m_lockManager->readWrite(work->id()));

	if (data.priority() < work->priority())
		work->setPriority(data.priority());
}

void DefaultWorkFacade::schedule(Work &work)
{
	Work::Ptr p;

	do {
		RWLock::ScopedWriteLock guard(m_lock);

		auto it = m_storage.find(work.id());
		if (it != m_storage.end()) {
			p = it->second;
			apply(p, work);
		}
		else {
			p = new Work(work);
			m_storage.emplace(work.id(), p);
		}
	} while (0);

	m_scheduler->schedule(p);

	WorkReadGuard guard(m_lockManager->readOnly(work.id()));
	work = *p;
}

void DefaultWorkFacade::wakeup(Work &work)
{
	Work::Ptr p;

	do {
		RWLock::ScopedReadLock guard(m_lock);

		auto it = m_storage.find(work.id());
		if (it != m_storage.end())
			p = it->second;
		else
			throw NotFoundException("no such work to wakeup: " + work);
	} while (0);

	m_scheduler->wakeup(p);

	WorkReadGuard guard(m_lockManager->readOnly(work.id()));
	work = *p;
}

void DefaultWorkFacade::cancel(Work &work)
{
	Work::Ptr p;

	do {
		RWLock::ScopedReadLock guard(m_lock);

		auto it = m_storage.find(work.id());
		if (it != m_storage.end())
			p = it->second;
		else
			throw NotFoundException("no such work to cancel: " + work);
	} while (0);

	m_scheduler->cancel(p);

	WorkReadGuard guard(m_lockManager->readOnly(work.id()));
	work = *p;
}

bool DefaultWorkFacade::fetch(Work &work)
{
	RWLock::ScopedReadLock guard(m_lock);

	auto it = m_storage.find(work.id());
	if (it != m_storage.end()) {
		WorkReadGuard guard(m_lockManager->readOnly(work.id()));
		work = *(it->second);
		return true;
	}

	return false;
}

bool DefaultWorkFacade::remove(const Work &work)
{
	RWLock::ScopedWriteLock guard(m_lock);

	auto it = m_storage.find(work.id());
	if (it != m_storage.end()) {
		m_storage.erase(it);
		return true;
	}

	return false;
}
