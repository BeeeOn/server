#include "work/ExtendedWorkExecutor.h"

using namespace BeeeOn;

ExtendedWorkExecutor::~ExtendedWorkExecutor()
{
}

void ExtendedWorkExecutor::setScheduler(WorkScheduler::Ptr scheduler)
{
	m_scheduler = scheduler;
}

WorkScheduler::Ptr ExtendedWorkExecutor::scheduler() const
{
	return m_scheduler;
}

void ExtendedWorkExecutor::setLockManager(WorkLockManager::Ptr lockManager)
{
	m_lockManager = lockManager;
}

WorkLockManager::Ptr ExtendedWorkExecutor::lockManager() const
{
	return m_lockManager;
}
