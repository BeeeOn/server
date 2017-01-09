#include <Poco/SingletonHolder.h>
#include <Poco/Exception.h>

#include "dao/NoTransactionManager.h"

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
class NoTransaction : public ThreadLocalTransaction, TransactionImpl {
public:
	NoTransaction(ThreadLocal<ThreadLocalTransaction *> &self):
		ThreadLocalTransaction(*this, self)
	{
	}

	void commit()
	{
	}

	void rollback()
	{
	}

	void isolate(Transaction::Isolation mask)
	{
	}
};

NoTransactionManager::NoTransactionManager()
{
}

NoTransactionManager::~NoTransactionManager()
{
}

TransactionManager &NoTransactionManager::instance()
{
	static SingletonHolder<NoTransactionManager> singleton;
	return *singleton.get();
}

void NoTransactionManager::create(Poco::ThreadLocal<ThreadLocalTransaction *> &ref)
{
	new NoTransaction(ref);
}
