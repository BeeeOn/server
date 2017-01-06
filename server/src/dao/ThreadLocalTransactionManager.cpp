#include <Poco/Exception.h>

#include "dao/ThreadLocalTransactionManager.h"

using namespace Poco;
using namespace BeeeOn;

ThreadLocalTransaction::ThreadLocalTransaction(
		TransactionImpl &impl,
		ThreadLocal<ThreadLocalTransaction *> &self):
	Transaction(impl),
	m_self(self)
{
	if (m_self.get() != NULL) {
		// this is a bug probably in ThreadLocalTransactionManager
		// as it MUST NOT create a new transaction if one already exists
		throw IllegalStateException("transaction (" + m_self.get()->name() + ") already exists");
	}

	m_self.get() = this;
}

/**
 * Anytime a ThreadLocalTransaction is being destructed, we must ensure
 * that it is not referenced by the ThreadLocal instance. However, a
 * different transaction may be already there (if there is a use case
 * for such situation).
 */
ThreadLocalTransaction::~ThreadLocalTransaction()
{
	if (m_self.get() == this)
		m_self.get() = NULL;
}

ThreadLocalTransactionManager::~ThreadLocalTransactionManager()
{
}

/**
 * Access to the ThreadLocal memory.
 */
static ThreadLocal<ThreadLocalTransaction *> &impl()
{
	static ThreadLocal<ThreadLocalTransaction *> null;
	return null;
}

Transaction *ThreadLocalTransactionManager::start()
{
	ThreadLocal<ThreadLocalTransaction *> &ref = impl();

	if (ref.get() == NULL)
		create(ref);

	return ref.get();
}

Transaction *ThreadLocalTransactionManager::current()
{
	return impl().get();
}
