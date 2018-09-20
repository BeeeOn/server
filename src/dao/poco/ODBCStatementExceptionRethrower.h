#pragma once

#include <Poco/Data/ODBC/ODBCException.h>

#include "dao/poco/StatementExceptionRethrower.h"

namespace BeeeOn {

/**
 * @brief Handle Poco::Data::ODBC::StatementException in a way
 * that is more readable for debugging. It does not duplicate
 * information and is quite brief.
 */
class ODBCStatementExceptionRethrower : public StatementExceptionRethrower {
public:
	/**
	 * @brief Rethrow the given exception but improve its message
	 * according to data contained in the ODBC-specific exception
	 * instance. The statement internals are included.
	 */
	void rethrow(
		const Poco::Data::Statement &statement,
		const Poco::Exception &e) override;

protected:
	void rethrowODBC(
		const Poco::Data::Statement &statement,
		const Poco::Data::ODBC::StatementException &e) const;
};

}
