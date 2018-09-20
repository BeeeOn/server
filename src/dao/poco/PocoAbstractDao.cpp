#include <Poco/Exception.h>

#include "dao/QueryLoader.h"
#include "dao/Query.h"
#include "dao/poco/PocoTransactionImpl.h"
#include "dao/poco/PocoAbstractDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "dao/poco/StatementExceptionRethrower.h"
#include "dao/poco/PocoStatementInfo.h"
#include "transaction/TransactionManager.h"
#include "util/Backtrace.h"
#include "util/ClassInfo.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

PocoAbstractDao::PocoAbstractDao():
	m_manager(NULL),
	m_loader(NULL)
{
}

PocoAbstractDao::~PocoAbstractDao()
{
}

void PocoAbstractDao::setDaoManager(PocoDaoManager *manager)
{
	m_manager = manager;
}

void PocoAbstractDao::setTransactionManager(TransactionManager::Ptr manager)
{
	m_transactionManager = manager;
}

void PocoAbstractDao::setQueryLoader(QueryLoader::Ptr loader)
{
	m_loader = loader;
}

void PocoAbstractDao::setQueryProfiler(QueryProfiler::Ptr profiler)
{
	m_profiler = profiler;
}

PocoDaoManager &PocoAbstractDao::manager()
{
	if (m_manager)
		return *m_manager;

	throw IllegalStateException("missing manager");
}

Poco::Data::Session PocoAbstractDao::session(bool transact)
{
	Session session(openSession(transact));
	manager().customizeSession(session);

	return session;
}

Poco::Data::Session PocoAbstractDao::openSession(bool transact)
{
	Transaction *t = m_transactionManager->current();

	if (t == NULL) {
		if (transact && logger().warning()) {
			logger().warning("session is out of transaction management",
					__FILE__, __LINE__);
		}

		return manager().pool().get();
	}
	else {
		try {
			return t->impl<PocoTransactionImpl>().session();
		}
		catch (std::bad_cast &e) {
			throw IllegalStateException(
				"incompatible transaction, expected BeeeOn::PocoTransactionImpl");
		}
	}
}

void PocoAbstractDao::registerQuery(Query &query)
{
	m_queries.push_back(&query);
}

size_t PocoAbstractDao::execute(Statement &sql)
{
	size_t result;

	try {
		result = sql.execute();

		if (logger().trace()) {
			ostringstream buffer;

			buffer << "[result: " << result << "] ";
			PocoStatementInfo info(sql);
			info.dump(buffer);

			logger().trace(buffer.str(), __FILE__, __LINE__);
		}
		else if (logger().debug()) {
			string prefix("[result: ");
			prefix += to_string(result) + "] ";
			logger().debug(prefix + sql.toString(),
					__FILE__, __LINE__);
		}

		return result;
	}
	catch (const Exception &e) {
		StatementExceptionRethrower::rethrowAlways(sql, e);
		throw;
	}
}

RecordSet PocoAbstractDao::executeSelect(Statement &sql)
{
	execute(sql);
	RecordSet result(sql);
	return result;
}

void PocoAbstractDao::throwMissingId(const type_info &t)
{
	Backtrace trace;
	logger().critical(trace.toString());

	throw InvalidArgumentException("missing id for "
			+ ClassInfo(t).name());
}

double PocoAbstractDao::nanWhenEmpty(const Poco::Dynamic::Var &v)
{
	return v.isEmpty()? (double) NAN : v.convert<double>();
}

std::string PocoAbstractDao::emptyWhenNull(const Poco::Dynamic::Var &v)
{
	return v.isEmpty()? "" : v.toString();
}

bool PocoAbstractDao::hasColumn(const RecordSet &result, const std::string &name)
{
	const size_t count = result.columnCount();

	for (size_t i = 0; i < count; ++i) {
		if (result.columnName(i) == name)
			return true;
	}

	return false;
}

bool PocoAbstractDao::hasColumn(const Row &result, const std::string &name)
{
	const Row::NameVecPtr columns = result.names();

	for (auto column : *columns) {
		if (name == column)
			return true;
	}

	return false;
}

void PocoAbstractDao::loadQueries()
{
	for (auto query : m_queries) {
		logger().debug("loading query " + query->key(),
				__FILE__, __LINE__);

		query->load(*m_loader);
		query->setProfiler(m_profiler);
	}
}

void PocoAbstractDao::clearQueries()
{
	m_queries.clear();
}
