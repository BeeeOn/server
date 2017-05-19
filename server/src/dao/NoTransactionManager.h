#ifndef BEEEON_NO_TRANSACTION_MANAGER_H
#define BEEEON_NO_TRANSACTION_MANAGER_H

#include "dao/ThreadLocalTransactionManager.h"

namespace BeeeOn {

/**
 * A NoTransactionManager is a transaction manager that
 * does not provide any transactions. All created transactions
 * are empty and does not guarantee anything. This is useful
 * for testing.
 */
class NoTransactionManager : public ThreadLocalTransactionManager {
public:
	NoTransactionManager();
	~NoTransactionManager();

	static TransactionManager &instance();

protected:
	Transaction *create();
};

}

#endif
