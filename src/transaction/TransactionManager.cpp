#include <string>
#include <Poco/Exception.h>

#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TransactionImpl::~TransactionImpl()
{
}

const string TransactionImpl::name() const
{
	return to_string((unsigned long) ((void *) this));
}

void Transaction::isolate(Transaction::Isolation)
{
	throw NotImplementedException("isolate()");
}

Transaction::~Transaction()
{
}

AbstractTransaction::AbstractTransaction(TransactionImpl &impl):
	m_impl(impl)
{
}

TransactionImpl &AbstractTransaction::impl(const type_info &info)
{
	if (typeid(m_impl) != info)
		throw BadCastException("bad TransactionImpl requested");

	return m_impl;
}

const string AbstractTransaction::name() const
{
	return m_impl.name();
}

TransactionFactory::~TransactionFactory()
{
}

TransactionManager::~TransactionManager()
{
}
