#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>

#include "dao/poco/PocoDBInitializer.h"

using namespace std;
using namespace Poco::Data;
using namespace BeeeOn;

unsigned long PocoDBInitializer::execute(
		Session &session, const string &script) const
{
	Statement sql = (session << script);
	return sql.execute();
}
