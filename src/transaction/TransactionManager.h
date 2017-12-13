#ifndef BEEEON_TRANSACTION_MANAGER_H
#define BEEEON_TRANSACTION_MANAGER_H

#include <string>
#include <typeinfo>

#include <Poco/SharedPtr.h>

namespace BeeeOn {

/**
 * Abstract class to be used by the backend implementation.
 * It can expose implementation details when needed. It is
 * not intended to be used by the application code.
 */
class TransactionImpl {
public:
	virtual ~TransactionImpl();

	const std::string name() const;
};

/**
 * An interface to a transaction implementation. The API of
 * this class is public to the application code.
 */
class Transaction {
public:
	virtual ~Transaction();

	/**
	 * Commit the transaction.
	 */
	virtual void commit() = 0;

	/**
	 * Rollback the transaction.
	 */
	virtual void rollback() = 0;

	enum Isolation {
		READ_UNCOMMITTED = 0x01,
		READ_COMMITTED = 0x02,
		REPEATABLE_READ = 0x04,
		SERIALIZABLE = 0x08,
	};

	/**
	 * Isolate with the requested isolation mask.
	 * If it is not possible to isolate as requested it throws an exception.
	 */
	virtual void isolate(Isolation mask);

	/**
	 * Provide implementation details to the backend code.
	 */
	template <typename T>
	T &impl()
	{
		return dynamic_cast<T &>(impl(typeid(T)));
	}

	virtual TransactionImpl &impl(const std::type_info &type) = 0;

	/**
	 * Unreliable identification of the transaction.
	 * It is useful for debugging.
	 */
	virtual const std::string name() const = 0;
};

class AbstractTransaction : public Transaction {
public:
	AbstractTransaction(TransactionImpl &impl);

	TransactionImpl &impl(const std::type_info &type) override;
	const std::string name() const override;

private:
	TransactionImpl &m_impl;
};

/**
 * Factory for transactions specific to the target resource.
 */
class TransactionFactory {
public:
	typedef Poco::SharedPtr<TransactionFactory> Ptr;

	virtual ~TransactionFactory();

	/**
	 * Create a new transaction.
	 */
	virtual Transaction *create() = 0;
};

/**
 * Interface to a transaction manager providing transactions to
 * the application code. It is typically called by the service layer
 * anytime when it is necessary to access the current transaction.
 */
class TransactionManager {
public:
	typedef Poco::SharedPtr<TransactionManager> Ptr;

	virtual ~TransactionManager();

	/**
	 * Return the current transaction or start a new transaction
	 * depending on strategy.
	 *
	 * Always call delete on the returned instance after commit
	 * or rollback has been called.
	 */
	virtual Transaction *start() = 0;

	/**
	 * Return the current transaction if any or NULL.
	 */
	virtual Transaction *current() = 0;
};

}

#endif
