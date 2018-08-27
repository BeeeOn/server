#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/StringTokenizer.h>

#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>

#include "di/Injectable.h"
#include "dao/poco/PocoPerQueryScriptExecutor.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoPerQueryScriptExecutor)
BEEEON_OBJECT_CASTABLE(PocoScriptExecutor)
BEEEON_OBJECT_PROPERTY("allowGarbage", &PocoPerQueryScriptExecutor::setAllowGarbage)
BEEEON_OBJECT_END(BeeeOn, PocoPerQueryScriptExecutor)

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

static const char QUERY_DELIMITER = ';';

PocoPerQueryScriptExecutor::PocoPerQueryScriptExecutor()
{
}

void PocoPerQueryScriptExecutor::setAllowGarbage(bool allow)
{
	m_allowGarbage = allow;
}

vector<string> PocoPerQueryScriptExecutor::splitQueries(const string &script) const
{
	StringTokenizer lines(script, "\n");
	vector<string> queries;

	string query = "";

	for (auto const line : lines) {
		query += line;

		if (line.back() == QUERY_DELIMITER) {
			queries.emplace_back(query);
			query.clear();
		}
		else {
			query += "\n";
		}
	}

	if (!query.empty()) {
		if (logger().debug()) {
			logger().dump(
				"garbage without ';'",
				query.data(),
				query.size(),
				Message::PRIO_DEBUG);
		}

		if (!m_allowGarbage)
			throw SyntaxException("garbage at the and of script (no ';' found)");
	}

	return queries;
}

unsigned long PocoPerQueryScriptExecutor::execute(
		Session &session,
		const string &script) const
{
	const auto &queries = splitQueries(script);

	if (queries.empty()) {
		logger().notice(
			"no query to be executed",
			__FILE__, __LINE__);
		return 0;
	}
	else {
		logger().notice(
			"executing " + to_string(queries.size()) + " queries",
			__FILE__, __LINE__);
	}

	size_t i = 0;
	unsigned long total = 0;

	for (const auto &query : queries) {
		i += 1;

		if (logger().trace()) {
			logger().dump(
				"executing query "
				+ to_string(i) + "/" + to_string(queries.size()),
				query.data(),
				query.size(),
				Message::PRIO_TRACE);
		}
		else if (logger().debug()) {
			logger().debug(
				"executing query "
				+ to_string(i) + "/" + to_string(queries.size()),
				__FILE__, __LINE__);
		}

		const auto result = execute(session, query);
		total += result;

		if (logger().debug()) {
			logger().debug(
				"query result: " + to_string(result),
				__FILE__, __LINE__);
		}
	}

	return total;
}
