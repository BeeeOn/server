#include <Poco/Exception.h>

#include "transaction/Transactional.h"

using namespace BeeeOn;

Transactional::Transactional()
{
}

void Transactional::setTransactionManager(TransactionManager::Ptr manager)
{
	m_transactionManager = manager;
}

TransactionManager::Ptr Transactional::transactionManager() const
{
	return m_transactionManager;
}

void Transactional::transactionNotNull(const Transaction *t)
{
	if (t == NULL)
		throw Poco::NullPointerException("transaction is NULL");
}

const std::string Transactional::transactionStarted(const Transaction &t)
{
	return "transaction " + t.name() + " started";
}

const std::string Transactional::transactionCommit(const Transaction &t)
{
	return "transaction " + t.name() + " commit";
}

const std::string Transactional::transactionRollback(const Transaction &t)
{
	return "transaction " + t.name() + " rollback";
}
