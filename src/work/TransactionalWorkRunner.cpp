#include "di/Injectable.h"
#include "work/TransactionalWorkRunner.h"
#include "work/WorkExecutor.h"
#include "work/WorkLockManager.h"
#include "work/WorkSuspendThrowable.h"

BEEEON_OBJECT_BEGIN(BeeeOn, TransactionalWorkRunnerFactory)
BEEEON_OBJECT_CASTABLE(WorkRunnerFactory)
BEEEON_OBJECT_REF("transactionManager", &TransactionalWorkRunnerFactory::setTransactionManager)
BEEEON_OBJECT_REF("lockManager", &TransactionalWorkRunnerFactory::setLockManager)
BEEEON_OBJECT_END(BeeeOn, TransactionalWorkRunnerFactory)

using namespace Poco;
using namespace BeeeOn;

TransactionalWorkRunner::TransactionalWorkRunner(
		WorkScheduler &scheduler,
		WorkLockManager &manager):
	GenericWorkRunner(scheduler, manager)
{
}

void TransactionalWorkRunner::execute()
{
	WorkExecutionGuard guard(m_lockManager.execute(m_work->id()));
	WorkWriteGuard accessGuard(m_lockManager.readWrite(m_work->id()));

	try {
		prepare();
		BEEEON_TRANSACTION(doExecute(accessGuard));
	} catch (...) {
		doFailed();
		throw;
	}
}

void TransactionalWorkRunner::doExecute(WorkWriteGuard &guard)
{
	try {
		m_executor->execute(m_work);
		doFinish();
	}
	catch (const WorkSuspendForEventThrowable &t) {
		m_work->setNoSleepDuration();
		doSuspend(guard);
	}
	catch (const WorkSuspendThrowable &t) {
		m_work->setSleepDuration(t.duration());
		doSuspend(guard);
	}
}

TransactionalWorkRunnerFactory::TransactionalWorkRunnerFactory()
{
}

WorkRunner *TransactionalWorkRunnerFactory::create(
		WorkScheduler &scheduler)
{
	TransactionalWorkRunner *runner(new TransactionalWorkRunner(scheduler, *m_lockManager));
	runner->setTransactionManager(m_manager);

	return runner;
}

void TransactionalWorkRunnerFactory::setTransactionManager(TransactionManager::Ptr manager)
{
	m_manager = manager;
}
