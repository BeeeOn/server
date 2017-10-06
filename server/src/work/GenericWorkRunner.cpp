#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Timestamp.h>

#include "di/Injectable.h"
#include "work/GenericWorkRunner.h"
#include "work/WorkBackup.h"
#include "work/WorkExecutor.h"
#include "work/WorkLockManager.h"
#include "work/WorkScheduler.h"
#include "work/WorkSuspendThrowable.h"

BEEEON_OBJECT_BEGIN(BeeeOn, GenericWorkRunnerFactory)
BEEEON_OBJECT_CASTABLE(WorkRunnerFactory)
BEEEON_OBJECT_REF("lockManager", &WorkRunnerFactory::setLockManager)
BEEEON_OBJECT_END(BeeeOn, GenericWorkRunnerFactory)

using namespace Poco;
using namespace BeeeOn;

GenericWorkRunner::GenericWorkRunner(WorkScheduler &scheduler, WorkLockManager &lockManager):
	WorkRunner(scheduler, lockManager)
{
}

GenericWorkRunner::~GenericWorkRunner()
{
}

void GenericWorkRunner::doSuspend(WorkWriteGuard &guard)
{
	if (logger().debug()) {
		logger().debug("work " + *m_work + " will be suspended",
				__FILE__, __LINE__);
	}

	m_work->setSuspended(Timestamp());

	// Calling schedule while holding execution lock
	// this should not be an issue.
	// However, holding of access lock would cause a deadlock.
	guard.unlock();
	m_scheduler.schedule(m_work);
}

void GenericWorkRunner::doFinish()
{
	if (logger().debug()) {
		logger().debug("work " + *m_work + " has finished",
				__FILE__, __LINE__);
	}

	m_work->setState(Work::STATE_FINISHED);
	m_work->setFinished(Timestamp());
	m_backup->store(m_work);
}

void GenericWorkRunner::doFailed()
{
	if (logger().debug()) {
		logger().debug("work " + *m_work + " has failed",
				__FILE__, __LINE__);
	}

	m_work->setState(Work::STATE_FAILED);
	m_work->setFinished(Timestamp());
	m_backup->store(m_work);
}

void GenericWorkRunner::run()
{
	WorkScheduler &scheduler = m_scheduler;

	try {
		execute();
	}
	catch (const Poco::Exception &e) {
		logger().log(e, __FILE__, __LINE__);
	}
	catch (const std::exception &e) {
		logger().critical(e.what(), __FILE__, __LINE__);
	}
	catch (...) {
		logger().critical("work " + *m_work + " failed to execute",
				__FILE__, __LINE__);
	}

	destroySelf();
	scheduler.notify();
}

void GenericWorkRunner::prepare()
{
	m_work->setState(Work::STATE_EXECUTED);
	m_backup->store(m_work);
}

void GenericWorkRunner::execute()
{
	WorkExecutionGuard guard(m_lockManager.execute(m_work->id()));
	WorkWriteGuard accessGuard(m_lockManager.readWrite(m_work->id()));

	if (logger().debug())
		logger().debug("executing work " + *m_work, __FILE__, __LINE__);

	try {
		prepare();
		m_executor->execute(m_work);
		doFinish();
	}
	catch (const WorkSuspendForEventThrowable &t) {
		m_work->setNoSleepDuration();
		doSuspend(accessGuard);
	}
	catch (const WorkSuspendThrowable &t) {
		m_work->setSleepDuration(t.duration());
		doSuspend(accessGuard);
	}
	catch (...) {
		doFailed();
		throw;
	}
}

WorkRunner *GenericWorkRunnerFactory::create(
		WorkScheduler &scheduler)
{
	return new GenericWorkRunner(scheduler, *m_lockManager);
}
