#include <string>
#include <Poco/Exception.h>

#include "dao/TransactionManager.h"

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

Transaction::Transaction(TransactionImpl &impl):
	m_impl(impl)
{
}

void Transaction::isolate(Transaction::Isolation mask)
{
	throw NotImplementedException("isolate()");
}

Transaction::~Transaction()
{
}

TransactionManager::~TransactionManager()
{
}
