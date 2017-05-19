#ifndef BEEEON_THREAD_LOCAL_TRANSACTION_MANAGER_H
#define BEEEON_THREAD_LOCAL_TRANSACTION_MANAGER_H

#include <Poco/ThreadLocal.h>

#include "dao/TransactionManager.h"

namespace BeeeOn {

/**
 * A thread-local transaction is used in the most common transactional strategy.
 * Every thread has no or a single transaction active at anytime. This means
 * that we can access such transaction anytime in the code flow where we have
 * access to the appropriate TransactionManager instance.
 *
 * The ThreadLocalTransaction has the pointer to the ThreadLocal instance holding
 * it. When the ThreadLocalTransaction is destroyed (by delete), the reference
 * in the ThreadLocal is cleared.
 */
class ThreadLocalTransaction : public AbstractTransaction {
public:
	ThreadLocalTransaction(TransactionImpl &impl,
			Poco::ThreadLocal<ThreadLocalTransaction *> &self);
	virtual ~ThreadLocalTransaction();

private:
	Poco::ThreadLocal<ThreadLocalTransaction *> &m_self;
};

class ThreadLocalTransactionManager : public TransactionManager {
public:
	virtual ~ThreadLocalTransactionManager();

	Transaction *start();
	Transaction *current();

protected:
	virtual void create(Poco::ThreadLocal<ThreadLocalTransaction *> &ref) = 0;
};

}

#endif
