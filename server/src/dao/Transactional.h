#ifndef BEEEON_TRANSACTIONAL_H
#define BEEEON_TRANSACTIONAL_H

#include "di/InjectorTarget.h"
#include "dao/TransactionManager.h"
#include "util/Loggable.h"

namespace BeeeOn {

#define BEEEON_TRANSACTION(code) \
	this->transaction([&]() {code;});
#define BEEEON_TRANSACTION_RETURN(type, code) \
	this->transaction<type>([&]() {return code;});

class Transactional : public AbstractInjectorTarget,
		public Loggable {
public:
	Transactional();

	void setTransactionManager(TransactionManager *manager);

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
	TransactionManager *m_transactionManager;
};

}

#endif

