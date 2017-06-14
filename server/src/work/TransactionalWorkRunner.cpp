#include "di/Injectable.h"
#include "transaction/NullTransactionManager.h"
#include "work/TransactionalWorkRunner.h"
#include "work/WorkAccess.h"
#include "work/WorkExecutor.h"
#include "work/WorkSuspendThrowable.h"

BEEEON_OBJECT_BEGIN(BeeeOn, TransactionalWorkRunnerFactory)
BEEEON_OBJECT_CASTABLE(WorkRunnerFactory)
BEEEON_OBJECT_REF("transactionManager", &TransactionalWorkRunnerFactory::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, TransactionalWorkRunnerFactory)

using namespace Poco;
using namespace BeeeOn;

TransactionalWorkRunner::TransactionalWorkRunner(
		WorkScheduler &scheduler):
	GenericWorkRunner(scheduler)
{
}

void TransactionalWorkRunner::execute()
{
	WorkExecuting guard(m_work, __FILE__, __LINE__);

	try {
		prepare();
		BEEEON_TRANSACTION(doExecute(guard));
	} catch (...) {
		doFailed();
		throw;
	}
}

void TransactionalWorkRunner::doExecute(WorkExecuting &guard)
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

TransactionalWorkRunnerFactory::TransactionalWorkRunnerFactory():
	m_manager(&NullTransactionManager::instance())
{
}

WorkRunner *TransactionalWorkRunnerFactory::create(
		WorkScheduler &scheduler)
{
	TransactionalWorkRunner *runner(new TransactionalWorkRunner(scheduler));
	runner->setTransactionManager(m_manager);

	return runner;
}

void TransactionalWorkRunnerFactory::setTransactionManager(TransactionManager *manager)
{
	m_manager = manager? manager : &NullTransactionManager::instance();
}
