#pragma once

#include <Poco/ThreadLocal.h>

#include "transaction/TransactionManager.h"

namespace BeeeOn {

/**
 * A thread-local transaction is used in the most common transactional strategy.
 * Every thread has no or a single transaction active at anytime. This means
 * that we can access such transaction anytime in the code flow where we have
 * access to the appropriate TransactionManager instance.
 *
 * The ThreadLocalTransactionWrapper has the pointer to the ThreadLocal instance
 * holding it. When the ThreadLocalTransactionWrapper is destroyed (by delete),
 * the reference in the ThreadLocal is cleared.
 */
class ThreadLocalTransactionWrapper : public Transaction {
public:
	ThreadLocalTransactionWrapper
		(Poco::ThreadLocal<ThreadLocalTransactionWrapper *> &self);
	virtual ~ThreadLocalTransactionWrapper();

	void commit() override;
	void rollback() override;
	void isolate(Isolation mask) override;
	TransactionImpl &impl(const std::type_info &type) override;

	const std::string name() const override;

	void setTransaction(Transaction *transaction);
	Transaction *transaction();
	const Transaction *transaction() const;

protected:
	void assureHasTransaction() const;

private:
	Transaction *m_transaction;
	Poco::ThreadLocal<ThreadLocalTransactionWrapper *> &m_self;
};

class ThreadLocalTransactionManager : public TransactionManager {
public:
	ThreadLocalTransactionManager();
	virtual ~ThreadLocalTransactionManager();

	Transaction *start();
	Transaction *current();

	void setFactory(TransactionFactory::Ptr factory);

protected:
	virtual Transaction *create();

private:
	TransactionFactory::Ptr m_factory;
	Poco::ThreadLocal<ThreadLocalTransactionWrapper *> m_null;
};

}
