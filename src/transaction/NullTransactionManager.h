#ifndef BEEEON_NULL_TRANSACTION_MANAGER_H
#define BEEEON_NULL_TRANSACTION_MANAGER_H

#include "transaction/TransactionManager.h"

namespace BeeeOn {

/**
 * A NullTransactionManager is a singleton and can be used as a default
 * instance of a TransactionManager instead of NULL pointer.
 */
class NullTransactionManager : public TransactionManager {
public:
	NullTransactionManager();
	~NullTransactionManager();

	Transaction *start();
	Transaction *current();

	static TransactionManager &instance();
};

class NullTransactionFactory : public TransactionFactory {
public:
	Transaction *create() override;

	static TransactionFactory &instance();
};

}

#endif
