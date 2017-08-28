#ifndef BEEEON_WORK_LOCK_MANAGER_H
#define BEEEON_WORK_LOCK_MANAGER_H

#include <functional>

#include <Poco/Mutex.h>
#include <Poco/RWLock.h>
#include <Poco/SharedPtr.h>

#include "work/Work.h"
#include "util/HashedLock.h"
#include "util/Loggable.h"

namespace BeeeOn {

class WorkWriteGuard;
class WorkReadGuard;
class WorkExecutionGuard;

/**
 * Manager of locks for Work instances. The WorkLockManager is to be shared
 * among WorkScheduler, WorkRunner and all places that access the Work::Ptr
 * instances in the system.
 */
class WorkLockManager : Loggable {
public:
	typedef Poco::SharedPtr<WorkLockManager> Ptr;

	WorkLockManager();
	WorkLockManager(unsigned int accessCount, unsigned int executionCount);

	/**
	 * Lock work of the given ID for reading.
	 */
	WorkReadGuard readOnly(const WorkID &id);

	/**
	 * Lock work of the given ID for writing.
	 */
	WorkWriteGuard readWrite(const WorkID &id);

	/**
	 * Lock work of the given ID for execution.
	 */
	WorkExecutionGuard execute(const WorkID &id);

	/**
	 * Set count of locks for access lock manager.
	 */
	void setAccessLockCount(int count);

	/**
	 * Set count of locks for execution lock manager.
	 */
	void setExecutionLockCount(int count);

private:
	typedef HashedLock<Poco::RWLock, WorkID> AccessLock;
	AccessLock m_accessLock;
	typedef HashedLock<Poco::FastMutex, WorkID> ExecutionLock;
	ExecutionLock m_executionLock;
};

template <typename Lock>
class WorkGuard {
public:
	WorkGuard(const WorkID &id, Lock &lock, Poco::Logger &logger,
			std::function<void(Lock &lock)> unlock):
		m_id(id),
		m_lock(&lock),
		m_logger(logger),
		m_unlock(unlock)
	{
	}

	WorkGuard(const WorkGuard &) = delete;

	WorkGuard(WorkGuard &&from):
		m_id(from.m_id),
		m_lock(from.m_lock),
		m_logger(from.m_logger),
		m_unlock(from.m_unlock)
	{
		from.m_lock = NULL;
	}

	virtual ~WorkGuard()
	{
		unlock();
	}

	void unlock()
	{
		if (m_lock != NULL)
			m_unlock(*m_lock);
	}

protected:
	const WorkID &id() const
	{
		return m_id;
	}

	Poco::Logger &logger()
	{
		return m_logger;
	}

private:
	const WorkID m_id;
	Lock *m_lock;
	Poco::Logger &m_logger;
	std::function<void(Lock &lock)> m_unlock;
};

class WorkExecutionGuard : public WorkGuard<Poco::FastMutex> {
public:
	WorkExecutionGuard(const WorkID &id, Poco::FastMutex &lock, Poco::Logger &logger);
	WorkExecutionGuard(const WorkExecutionGuard &) = delete;
	WorkExecutionGuard(WorkExecutionGuard &&from);

protected:
	void doUnlock(Poco::FastMutex &lock);
};

class WorkWriteGuard : public WorkGuard<Poco::RWLock> {
public:
	WorkWriteGuard(const WorkID &id, Poco::RWLock &lock, Poco::Logger &logger);
	WorkWriteGuard(const WorkWriteGuard &) = delete;
	WorkWriteGuard(WorkWriteGuard &&from);

protected:
	void doUnlock(Poco::RWLock &lock);
};

class WorkReadGuard : public WorkGuard<Poco::RWLock> {
public:
	WorkReadGuard(const WorkID &id, Poco::RWLock &lock, Poco::Logger &logger);
	WorkReadGuard(const WorkReadGuard &) = delete;
	WorkReadGuard(WorkReadGuard &&from);

protected:
	void doUnlock(Poco::RWLock &lock);
};

}

#endif
