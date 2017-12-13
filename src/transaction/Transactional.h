#ifndef BEEEON_TRANSACTIONAL_H
#define BEEEON_TRANSACTIONAL_H

#include <Poco/Logger.h>

#include "transaction/TransactionManager.h"
#include "util/Loggable.h"

namespace BeeeOn {

#define BEEEON_TRANSACTION(code) \
	BEEEON_TRANSACTION_ON(*this, code)
#define BEEEON_TRANSACTION_RETURN(type, code) \
	BEEEON_TRANSACTION_RETURN_ON(*this, type, code)

#define BEEEON_TRANSACTION_ON(transactional, code) \
	(transactional).transaction([&]() {code;})
#define BEEEON_TRANSACTION_RETURN_ON(transactional, type, code) \
	(transactional).transaction<type>([&]() {return code;})

class Transactional : public Loggable {
public:
	Transactional();

	template <typename Wrapper>
	Transactional(const Wrapper *):
		Loggable(typeid(Wrapper))
	{
	}

	void setTransactionManager(TransactionManager::Ptr manager);
	TransactionManager::Ptr transactionManager() const;

	template <typename Ret, typename A>
	Ret transaction(const A &action)
	{
		Transaction *t = m_transactionManager->start();
		logTransactionStarted(t, __FILE__, __LINE__);

		try {
			const Ret &ret = action();

			t->commit();
			logTransactionCommit(t, __FILE__, __LINE__);
			delete t;

			return ret;
		} catch (...) {
			t->rollback();
			logTransactionRollback(t, __FILE__, __LINE__);
			delete t;

			throw;
		}
	}

	template <typename A>
	void transaction(const A &action)
	{
		Transaction *t = m_transactionManager->start();
		logTransactionStarted(t, __FILE__, __LINE__);

		try {
			action();

			t->commit();
			logTransactionCommit(t, __FILE__, __LINE__);
			delete t;
		} catch (...) {
			t->rollback();
			logTransactionRollback(t, __FILE__, __LINE__);
			delete t;

			throw;
		}
	}

protected:
	void logTransactionStarted(const Transaction *t,
			const char *file, unsigned int line)
	{
		transactionNotNull(t);

		if (logger().debug())
			logger().debug(transactionStarted(*t), file, line);
	}

	void logTransactionCommit(const Transaction *t,
			const char *file, unsigned int line)
	{
		transactionNotNull(t);

		if (logger().debug())
			logger().debug(transactionCommit(*t), file, line);
	}

	void logTransactionRollback(const Transaction *t,
			const char *file, unsigned int line)
	{
		transactionNotNull(t);

		if (logger().error())
			logger().error(transactionRollback(*t), file, line);
	}

	void transactionNotNull(const Transaction *t);
	const std::string transactionStarted(const Transaction &t);
	const std::string transactionCommit(const Transaction &t);
	const std::string transactionRollback(const Transaction &t);

private:
	TransactionManager::Ptr m_transactionManager;
};

}

#endif

