#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>

#include "dao/poco/PocoTransactionImpl.h"

using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;
using Isolation = BeeeOn::Transaction::Isolation;

PocoTransactionImpl::PocoTransactionImpl(SessionPool &pool):
	m_session(pool.get()),
	m_transaction(m_session, true)
{
}

PocoTransactionImpl::~PocoTransactionImpl()
{
}

PocoTransaction::PocoTransaction(SessionPool &pool,
		ThreadLocal<ThreadLocalTransaction *> &ref):
	 // we assume that ThreadLocalTransaction does not touch m_impl yet
	ThreadLocalTransaction(m_impl, ref),
	m_impl(pool)
{
}

void PocoTransaction::commit()
{
	m_impl.transaction().commit();
}

void PocoTransaction::rollback()
{
	m_impl.transaction().rollback();
}

static Poco::UInt32 convertMask(Isolation mask)
{
	Poco::UInt32 value = 0;

	if (mask & Isolation::READ_COMMITTED)
		value |= Session::TRANSACTION_READ_COMMITTED;
	if (mask & Isolation::READ_UNCOMMITTED)
		value |= Session::TRANSACTION_READ_UNCOMMITTED;
	if (mask & Isolation::REPEATABLE_READ)
		value |= Session::TRANSACTION_REPEATABLE_READ;
	if (mask & Isolation::SERIALIZABLE)
		value |= Session::TRANSACTION_SERIALIZABLE;

	return value;
}

void PocoTransaction::isolate(Isolation mask)
{
	m_impl.session().setTransactionIsolation(convertMask(mask));
}
