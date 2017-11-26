#include <Poco/SingletonHolder.h>
#include <Poco/Exception.h>

#include "transaction/NoTransactionFactory.h"

using namespace Poco;
using namespace BeeeOn;

/**
 * NoTransaction implements empty commit, rollback and isolate operations.
 *
 * There currently no way how to put a TransactionImpl from
 * another implementation (however, it might be possible
 * for some use cases). Thus, the NoTransaction is a TransactionImpl
 * for itself.
 */
class NoTransaction : public AbstractTransaction, TransactionImpl {
public:
	NoTransaction():
		AbstractTransaction(static_cast<TransactionImpl &>(*this))
	{
	}

	void commit()
	{
	}

	void rollback()
	{
	}

	void isolate(Transaction::Isolation)
	{
	}
};

NoTransactionFactory::NoTransactionFactory()
{
}

NoTransactionFactory::~NoTransactionFactory()
{
}

TransactionFactory &NoTransactionFactory::instance()
{
	static SingletonHolder<NoTransactionFactory> singleton;
	return *singleton.get();
}

Transaction *NoTransactionFactory::create()
{
	return new NoTransaction;
}
