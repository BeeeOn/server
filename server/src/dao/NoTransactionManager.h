#ifndef BEEEON_NO_TRANSACTION_MANAGER_H
#define BEEEON_NO_TRANSACTION_MANAGER_H

#include "di/AbstractInjectorTarget.h"
#include "dao/ThreadLocalTransactionManager.h"

namespace BeeeOn {

/**
 * A NoTransactionManager is a transaction manager that
 * does not provide any transactions. All created transactions
 * are empty and does not guarantee anything. This is useful
 * for testing.
 */
class NoTransactionManager :
		public ThreadLocalTransactionManager,
		public AbstractInjectorTarget {
public:
	NoTransactionManager();
	~NoTransactionManager();

	static TransactionManager &instance();

protected:
	void create(Poco::ThreadLocal<ThreadLocalTransaction *> &ref);
};

}

#endif
