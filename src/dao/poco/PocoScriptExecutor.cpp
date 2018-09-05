#include <Poco/Data/Statement.h>

#include "di/Injectable.h"
#include "dao/poco/PocoScriptExecutor.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoDefaultScriptExecutor)
BEEEON_OBJECT_CASTABLE(PocoScriptExecutor)
BEEEON_OBJECT_END(BeeeOn, PocoDefaultScriptExecutor)

using namespace std;
using namespace BeeeOn;
using namespace Poco::Data;

PocoScriptExecutor::~PocoScriptExecutor()
{
}

unsigned long PocoDefaultScriptExecutor::execute(
		Session &session,
		const string &script) const
{
	Statement stmt = (session << script);
	return stmt.execute();
}
