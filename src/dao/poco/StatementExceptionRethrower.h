#pragma once

#include <vector>

#include <Poco/Exception.h>
#include <Poco/SharedPtr.h>
#include <Poco/Data/Statement.h>

namespace BeeeOn {

/**
 * @brief StatementExceptionRethrower allows to reformat exception
 * messages with respect to the associated statement and rethrow
 * such exceptions in new better form.
 *
 * The database exceptions can be of an implementation-specific type.
 * Such exceptions are usually not linked into the main application
 * (and loaded dynamically). Thus, we are not able to hard-wire their
 * processing into the code. It is possible to register specific
 * instances of StatementExceptionRethrower during runtime to handle
 * exceptions from external libraries.
 */
class StatementExceptionRethrower {
public:
	typedef Poco::SharedPtr<StatementExceptionRethrower> Ptr;

	virtual ~StatementExceptionRethrower();

	/**
	 * @brief Preprocess the given exception in the context
	 * of the given statement and rethrow it. If it does not
	 * understand that exception, just return.
	 */
	virtual void rethrow(
		const Poco::Data::Statement &statement,
		const Poco::Exception &e) = 0;

	/**
	 * @brief Register a new rethrower globally.
	 */
	static void registerRethrower(StatementExceptionRethrower::Ptr rethrower);

	/**
	 * @brief Handle the given statement and exception via the
	 * registered rethrowers chain. The method would always rethrow.
	 */
	static void rethrowAlways(
		const Poco::Data::Statement &statement,
		const Poco::Exception &e);

protected:
	/**
	 * @brief Most implementations of StatementExceptionRethrower::rethrow()
	 * are usually very similar. This method finishes the rethrowing process
	 * and does the actual rethrow (Poco::Exception::rethrow()).
	 */
	static void finishRethrowException(
		const std::string &message,
		const Poco::Exception &e);

	/**
	 * @brief Generic implementation of StatementExceptionRethrower::rethrow()
	 * that is used when no rethrowers matches the given exception.
	 */
	static void defaultRethrow(
		const Poco::Data::Statement &statement,
		const Poco::Exception &e);

private:
	static std::vector<StatementExceptionRethrower::Ptr> g_rethrowers;
};

}
