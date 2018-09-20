#include <sstream>

#include <Poco/String.h>

#include "dao/poco/PocoStatementInfo.h"
#include "dao/poco/StatementExceptionRethrower.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

vector<StatementExceptionRethrower::Ptr> StatementExceptionRethrower::g_rethrowers;

StatementExceptionRethrower::~StatementExceptionRethrower()
{
}

void StatementExceptionRethrower::registerRethrower(
		StatementExceptionRethrower::Ptr rethrower)
{
	g_rethrowers.emplace_back(rethrower);
}

void StatementExceptionRethrower::rethrowAlways(
		const Statement &statement,
		const Exception &e)
{
	for (auto rethrower : g_rethrowers)
		rethrower->rethrow(statement, e);

	defaultRethrow(statement, e);
}

void StatementExceptionRethrower::finishRethrowException(
		const string &message,
		const Exception &e)
{
	Poco::Data::DataException exception(message, e);
	exception.rethrow();
}

void StatementExceptionRethrower::defaultRethrow(
		const Statement &statement,
		const Exception &e)
{
	ostringstream buffer;

	buffer << trimRight(e.displayText()) << endl;

	PocoStatementInfo info(statement);
	info.dump(buffer);
	finishRethrowException(buffer.str(), e);
}
