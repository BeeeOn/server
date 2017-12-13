#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "transaction/ThreadLocalTransactionManager.h"

BEEEON_OBJECT_BEGIN(BeeeOn, ThreadLocalTransactionManager)
BEEEON_OBJECT_CASTABLE(TransactionManager)
BEEEON_OBJECT_REF("factory", &ThreadLocalTransactionManager::setFactory)
BEEEON_OBJECT_END(BeeeOn, ThreadLocalTransactionManager)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ThreadLocalTransactionWrapper::ThreadLocalTransactionWrapper(
		ThreadLocal<ThreadLocalTransactionWrapper *> &self):
	m_transaction(NULL),
	m_self(self)
{
	if (m_self.get() != NULL) {
		// this is a bug probably in ThreadLocalTransactionManager
		// as it MUST NOT create a new transaction if one already exists
		throw IllegalStateException("transaction (" + m_self.get()->name() + ") already exists");
	}

	m_self.get() = this;
}

const string ThreadLocalTransactionWrapper::name() const
{
	return transaction()->name();
}

void ThreadLocalTransactionWrapper::setTransaction(Transaction *transaction)
{
	m_transaction = transaction;
}

void ThreadLocalTransactionWrapper::assureHasTransaction() const
{
	if (m_transaction == NULL)
		throw NullPointerException("transaction is null");
}

const Transaction *ThreadLocalTransactionWrapper::transaction() const
{
	assureHasTransaction();
	return m_transaction;
}

Transaction *ThreadLocalTransactionWrapper::transaction()
{
	assureHasTransaction();
	return m_transaction;
}

void ThreadLocalTransactionWrapper::commit()
{
	transaction()->commit();
}

void ThreadLocalTransactionWrapper::rollback()
{
	transaction()->rollback();
}

void ThreadLocalTransactionWrapper::isolate(Isolation mask)
{
	transaction()->isolate(mask);
}

TransactionImpl &ThreadLocalTransactionWrapper::impl(const type_info &type)
{
	return transaction()->impl(type);
}

/**
 * Anytime a ThreadLocalTransactionWrapper is being destructed, we must
 * ensure that it is not referenced by the ThreadLocal instance. However,
 * a different transaction may be already there (if there is a use case
 * for such situation).
 */
ThreadLocalTransactionWrapper::~ThreadLocalTransactionWrapper()
{
	if (m_self.get() == this)
		m_self.get() = NULL;

	if (m_transaction != NULL)
		delete m_transaction;
}

ThreadLocalTransactionManager::ThreadLocalTransactionManager()
{
}

ThreadLocalTransactionManager::~ThreadLocalTransactionManager()
{
}

void ThreadLocalTransactionManager::setFactory(TransactionFactory::Ptr factory)
{
	m_factory = factory;
}

Transaction *ThreadLocalTransactionManager::start()
{
	ThreadLocal<ThreadLocalTransactionWrapper *> &ref = m_null;

	if (ref.get() == NULL) {
		(new ThreadLocalTransactionWrapper(ref))->setTransaction(create());
		return ref.get();
	}

	throw IllegalStateException("transaction already exists in this context");
}

Transaction *ThreadLocalTransactionManager::create()
{
	return m_factory->create();
}

Transaction *ThreadLocalTransactionManager::current()
{
	return m_null.get();
}
