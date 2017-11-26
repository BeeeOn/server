#include <Poco/Exception.h>

#include "transaction/NullTransactionManager.h"
#include "transaction/Transactional.h"

using namespace BeeeOn;

Transactional::Transactional():
	m_transactionManager(&NullTransactionManager::instance())
{
}

void Transactional::setTransactionManager(TransactionManager *manager)
{
	m_transactionManager = manager == NULL?
		&NullTransactionManager::instance() : manager;
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
