#pragma once

#include "transaction/TransactionManager.h"

namespace BeeeOn {

/**
 * A NoTransactionFactory is a transaction factory that
 * does not provide any transactions. All created transactions
 * are empty and does not guarantee anything. This is useful
 * for testing.
 */
class NoTransactionFactory : public TransactionFactory {
public:
	NoTransactionFactory();
	~NoTransactionFactory();

	Transaction *create() override;

	static TransactionFactory &instance();
};

}
