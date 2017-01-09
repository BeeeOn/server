#ifndef BEEEON_POCO_TRANSACTION_IMPL_H
#define BEEEON_POCO_TRANSACTION_IMPL_H

#include <Poco/Data/Session.h>
#include <Poco/Data/Transaction.h>

#include "dao/ThreadLocalTransactionManager.h"

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

class PocoTransaction : public ThreadLocalTransaction {
public:
	PocoTransaction(Poco::Data::SessionPool &pool,
			Poco::ThreadLocal<ThreadLocalTransaction *> &ref);

	void commit();
	void rollback();
	void isolate(Transaction::Isolation mask);

private:
	PocoTransactionImpl m_impl;
};

}

#endif
