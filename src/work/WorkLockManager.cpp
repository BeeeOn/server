#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "work/WorkLockManager.h"

BEEEON_OBJECT_BEGIN(BeeeOn, WorkLockManager)
BEEEON_OBJECT_NUMBER("accessLockCount", &WorkLockManager::setAccessLockCount)
BEEEON_OBJECT_NUMBER("executionLockCount", &WorkLockManager::setExecutionLockCount)
BEEEON_OBJECT_END(BeeeOn, WorkLockManager)

using namespace Poco;
using namespace BeeeOn;

WorkLockManager::WorkLockManager():
	WorkLockManager(0, 0)
{
}

WorkLockManager::WorkLockManager(unsigned int accessCount, unsigned int executionCount):
	m_accessLock(accessCount),
	m_executionLock(executionCount)
{
}

void WorkLockManager::setAccessLockCount(int count)
{
	if (count <= 0)
		throw InvalidArgumentException("count of locks must be positive");

	m_accessLock.delayedInit((unsigned int) count);
}

void WorkLockManager::setExecutionLockCount(int count)
{
	if (count <= 0)
		throw InvalidArgumentException("count of locks must be positive");

	m_executionLock.delayedInit((unsigned int) count);
}

WorkReadGuard WorkLockManager::readOnly(const WorkID &id)
{
	if (logger().trace()) {
		logger().trace("lock read-only work " + id.toString(),
			__FILE__, __LINE__);
	}

	return WorkReadGuard(id, m_accessLock.find(id), logger());
}

WorkWriteGuard WorkLockManager::readWrite(const WorkID &id)
{
	if (logger().trace()) {
		logger().trace("lock read-write work " + id.toString(),
			__FILE__, __LINE__);
	}

	return WorkWriteGuard(id, m_accessLock.find(id), logger());
}

WorkExecutionGuard WorkLockManager::execute(const WorkID &id)
{
	if (logger().trace()) {
		logger().trace("lock execution work " + id.toString(),
			__FILE__, __LINE__);
	}

	return WorkExecutionGuard(id, m_executionLock.find(id), logger());
}

WorkExecutionGuard::WorkExecutionGuard(const WorkID &id, FastMutex &lock, Logger &logger):
	WorkGuard<FastMutex>(id, lock, logger,
		[&](FastMutex &lock) {
			doUnlock(lock);	
		})
{
	lock.lock();
}

WorkExecutionGuard::WorkExecutionGuard(WorkExecutionGuard &&from):
	WorkGuard<FastMutex>(std::move(from))
{
}

void WorkExecutionGuard::doUnlock(FastMutex &lock)
{
	if (logger().trace()) {
		logger().trace("unlock execute work " + id().toString(),
			__FILE__, __LINE__);
	}

	lock.unlock();
}

WorkWriteGuard::WorkWriteGuard(const WorkID &id, RWLock &lock, Logger &logger):
	WorkGuard<RWLock>(id, lock, logger,
		[&](RWLock &lock) {
			doUnlock(lock);	
		})
{
	lock.writeLock();
}

WorkWriteGuard::WorkWriteGuard(WorkWriteGuard &&from):
	WorkGuard<RWLock>(std::move(from))
{
}

void WorkWriteGuard::doUnlock(RWLock &lock)
{
	if (logger().trace()) {
		logger().trace("unlock read-write work " + id().toString(),
			__FILE__, __LINE__);
	}

	lock.unlock();
}

WorkReadGuard::WorkReadGuard(const WorkID &id, RWLock &lock, Logger &logger):
	WorkGuard<RWLock>(id, lock, logger,
		[&](RWLock &lock) {
			doUnlock(lock);	
		})
{
	lock.readLock();
}

WorkReadGuard::WorkReadGuard(WorkReadGuard &&from):
	WorkGuard<RWLock>(std::move(from))
{
}

void WorkReadGuard::doUnlock(RWLock &lock)
{
	if (logger().trace()) {
		logger().trace("unlock read-only work " + id().toString(),
			__FILE__, __LINE__);
	}

	lock.unlock();
}
