#ifndef BEEEON_TRANSACTIONAL_WORK_RUNNER_H
#define BEEEON_TRANSACTIONAL_WORK_RUNNER_H

#include "dao/Transactional.h"
#include "work/GenericWorkRunner.h"

namespace BeeeOn {

class TransactionalWorkRunner :
	public Transactional,
	public GenericWorkRunner {
public:
	TransactionalWorkRunner(WorkScheduler &scheduler);

protected:
	void execute() override;
	void doExecute(WorkExecuting &guard);
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
