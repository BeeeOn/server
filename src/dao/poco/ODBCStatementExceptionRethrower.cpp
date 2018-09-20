#include <sstream>

#include "dao/poco/ODBCStatementExceptionRethrower.h"
#include "dao/poco/PocoStatementInfo.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::ODBC;
using namespace BeeeOn;

void ODBCStatementExceptionRethrower::rethrow(
		const Statement &stmt,
		const Exception &e)
{
	if (typeid(e) != typeid(StatementException))
		return;

	rethrowODBC(
		stmt,
		static_cast<const StatementException &>(e));
}

void ODBCStatementExceptionRethrower::rethrowODBC(
		const Statement &statement,
		const StatementException &e) const
{
	ostringstream buffer;

	buffer << "ODBC StatementException" << endl;

	const auto &diag = e.diagnostics();

	for (int i = 0; i < diag.count(); ++i) {
		buffer << "[" << i << "] ";
		buffer << "SQL state: " << diag.sqlState(i);
		buffer << ", ";
		buffer << "Native error: " << diag.nativeError(i);
		buffer << endl;
		buffer << trimRight(diag.message(i)) << endl;
	}

	PocoStatementInfo info(statement);
	info.dump(buffer);
	finishRethrowException(buffer.str(), e);
}

