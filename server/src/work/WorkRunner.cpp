#include <Poco/SingletonHolder.h>
#include <Poco/Exception.h>

#include "work/WorkRunner.h"
#include "work/WorkBackup.h"
#include "work/WorkExecutor.h"
#include "work/WorkLockManager.h"

using namespace Poco;
using namespace BeeeOn;

WorkRunner::WorkRunner(WorkScheduler &scheduler, WorkLockManager &lockManager):
	m_scheduler(scheduler),
	m_executor(&NullWorkExecutor::instance()),
	m_backup(&EmptyWorkBackup::instance()),
	m_lockManager(lockManager)
{
}

WorkRunner::~WorkRunner()
{
}

void WorkRunner::setExecutor(WorkExecutor *executor)
{
	m_executor = executor? executor : &NullWorkExecutor::instance();
}

void WorkRunner::setWork(Work::Ptr work)
{
	m_work = work;
}

void WorkRunner::setBackup(WorkBackup *backup)
{
	m_backup = backup? backup : &EmptyWorkBackup::instance();
}

void WorkRunner::destroySelf()
{
	delete this;
}

WorkRunnerFactory::~WorkRunnerFactory()
{
}

void WorkRunnerFactory::setLockManager(WorkLockManager *manager)
{
	m_lockManager = manager;
}

WorkRunner *NullWorkRunnerFactory::create(WorkScheduler &)
{
	throw NotImplementedException(__func__);
}

WorkRunnerFactory &NullWorkRunnerFactory::instance()
{
	static SingletonHolder<NullWorkRunnerFactory> singleton;
	return *singleton.get();
}
