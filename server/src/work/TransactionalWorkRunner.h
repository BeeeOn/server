#ifndef BEEEON_TRANSACTIONAL_WORK_RUNNER_H
#define BEEEON_TRANSACTIONAL_WORK_RUNNER_H

#include "transaction/Transactional.h"
#include "work/GenericWorkRunner.h"

namespace BeeeOn {

class WorkWriteGuard;

class TransactionalWorkRunner :
	public Transactional,
	public GenericWorkRunner {
public:
	TransactionalWorkRunner(WorkScheduler &scheduler, WorkLockManager &manager);

protected:
	void execute() override;
	void doExecute(WorkWriteGuard &guard);
};

class TransactionalWorkRunnerFactory : public WorkRunnerFactory {
public:
	TransactionalWorkRunnerFactory();

	WorkRunner *create(WorkScheduler &scheduler) override;

	void setTransactionManager(TransactionManager *manager);

private:
	TransactionManager *m_manager;
};

}

#endif
