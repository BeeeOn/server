#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "policy/PolicyContext.h"
#include "work/DefaultWorkFacade.h"
#include "work/WorkLockManager.h"
#include "work/WorkScheduler.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DefaultWorkFacade)
BEEEON_OBJECT_CASTABLE(WorkFacade)
BEEEON_OBJECT_REF("scheduler", &DefaultWorkFacade::setScheduler)
BEEEON_OBJECT_REF("lockManager", &DefaultWorkFacade::setLockManager)
BEEEON_OBJECT_REF("accessPolicy", &DefaultWorkFacade::setAccessPolicy)
BEEEON_OBJECT_END(BeeeOn, DefaultWorkFacade)

using namespace Poco;
using namespace BeeeOn;

DefaultWorkFacade::DefaultWorkFacade()
{
}

void DefaultWorkFacade::setScheduler(WorkScheduler::Ptr scheduler)
{
	m_scheduler = scheduler;
}

void DefaultWorkFacade::setLockManager(SharedPtr<WorkLockManager> manager)
{
	m_lockManager = manager;
}

void DefaultWorkFacade::setAccessPolicy(WorkAccessPolicy::Ptr policy)
{
	m_accessPolicy = policy;
}

const UserPolicyContext &DefaultWorkFacade::asUserContext(const PolicyContext &context) const
{
	if (context.is<UserPolicyContext>())
		return context.cast<UserPolicyContext>();

	throw IllegalStateException("given an incompatible policy context");
}

void DefaultWorkFacade::apply(Work::Ptr &work, const Work &data) const
{
	WorkWriteGuard guard(m_lockManager->readWrite(work->id()));

	if (data.priority() < work->priority())
		work->setPriority(data.priority());
}

void DefaultWorkFacade::schedule(Work &work, const PolicyContext &context)
{
	Work::Ptr p;

	{
		RWLock::ScopedWriteLock guard(m_lock);

		auto it = m_storage.find(work.id());
		if (it != m_storage.end()) {
			p = it->second;

			m_accessPolicy->assure(
				WorkAccessPolicy::ACTION_USER_SCHEDULE,
				context,
				*p
			);

			apply(p, work);
		}
		else {
			p = new Work(work);
			p->setOwner(asUserContext(context).user());

			m_accessPolicy->assure(
				WorkAccessPolicy::ACTION_USER_SCHEDULE,
				context,
				*p
			);

			m_storage.emplace(work.id(), p);
		}
	}

	m_scheduler->schedule(p);

	WorkReadGuard guard(m_lockManager->readOnly(work.id()));
	work = *p;
}

void DefaultWorkFacade::wakeup(Work &work, const PolicyContext &context)
{
	Work::Ptr p;

	{
		RWLock::ScopedReadLock guard(m_lock);

		auto it = m_storage.find(work.id());
		if (it != m_storage.end())
			p = it->second;
		else
			throw NotFoundException("no such work to wakeup: " + work);
	}

	{
		WorkReadGuard guard(m_lockManager->readOnly(work.id()));
		m_accessPolicy->assure(
			WorkAccessPolicy::ACTION_USER_WAKEUP,
			context,
			*p
		);
	}

	m_scheduler->wakeup(p);

	WorkReadGuard guard(m_lockManager->readOnly(work.id()));
	work = *p;
}

void DefaultWorkFacade::cancel(Work &work, const PolicyContext &context)
{
	Work::Ptr p;

	{
		RWLock::ScopedReadLock guard(m_lock);

		auto it = m_storage.find(work.id());
		if (it != m_storage.end())
			p = it->second;
		else
			throw NotFoundException("no such work to cancel: " + work);
	}

	{
		WorkReadGuard guard(m_lockManager->readOnly(work.id()));
		m_accessPolicy->assure(
			WorkAccessPolicy::ACTION_USER_CANCEL,
			context,
			*p
		);
	}

	m_scheduler->cancel(p);

	WorkReadGuard guard(m_lockManager->readOnly(work.id()));
	work = *p;
}

bool DefaultWorkFacade::fetch(Work &work, const PolicyContext &context)
{
	RWLock::ScopedReadLock guard(m_lock);

	auto it = m_storage.find(work.id());
	if (it != m_storage.end()) {
		WorkReadGuard guard(m_lockManager->readOnly(work.id()));
		Work::Ptr p = it->second;

		m_accessPolicy->assure(
			WorkAccessPolicy::ACTION_USER_CANCEL,
			context,
			*p
		);

		work = *p;
		return true;
	}

	return false;
}

bool DefaultWorkFacade::remove(const Work &work, const PolicyContext &context)
{
	RWLock::ScopedWriteLock guard(m_lock);

	auto it = m_storage.find(work.id());
	if (it != m_storage.end()) {
		WorkReadGuard guard(m_lockManager->readOnly(work.id()));
		Work::Ptr p = it->second;

		m_accessPolicy->assure(
			WorkAccessPolicy::ACTION_USER_REMOVE,
			context,
			*p
		);

		m_storage.erase(it);
		return true;
	}

	return false;
}
