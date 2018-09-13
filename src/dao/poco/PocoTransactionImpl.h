#pragma once

#include <Poco/Data/Session.h>
#include <Poco/Data/Transaction.h>

#include "transaction/TransactionManager.h"

namespace Poco {
namespace Data {

class SessionPool;

}
}

namespace BeeeOn {

class PocoTransactionImpl : public TransactionImpl {
public:
	PocoTransactionImpl(Poco::Data::SessionPool &pool);
	~PocoTransactionImpl();

	Poco::Data::Session &session()
	{
		return m_session;
	}

	Poco::Data::Transaction &transaction()
	{
		return m_transaction;
	}

private:
	Poco::Data::Session m_session;
	Poco::Data::Transaction m_transaction;
};

class PocoTransaction : public AbstractTransaction {
public:
	PocoTransaction(Poco::Data::SessionPool &pool);

	void commit() override;
	void rollback() override;
	void isolate(Transaction::Isolation mask) override;

private:
	PocoTransactionImpl m_impl;
};

}
