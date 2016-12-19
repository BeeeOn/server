#include <Poco/Exception.h>

#include "dao/SQLLoader.h"
#include "dao/poco/PocoAbstractDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

PocoAbstractDao::PocoAbstractDao():
	m_logger(LOGGER_CLASS(this)),
	m_manager(NULL),
	m_loader(NULL)
{
	injector<PocoAbstractDao, PocoDaoManager>(
		"daoManager",
		&PocoAbstractDao::setDaoManager
	);
	injector<PocoAbstractDao, SQLLoader>(
		"sqlLoader",
		&PocoAbstractDao::setSQLLoader
	);
}

PocoAbstractDao::~PocoAbstractDao()
{
}

void PocoAbstractDao::setDaoManager(PocoDaoManager *manager)
{
	m_manager = manager;
}

void PocoDaoManager::setSQLLoader(SQLLoader *loader)
{
	m_loader = loader;
}

PocoDaoManager &PocoAbstractDao::manager()
{
	if (m_manager)
		return *m_manager;

	throw IllegalStateException("missing manager");
}

string PocoAbstractDao::findQuery(const string &key) const
{
	if (m_loader)
		return m_loader->find(key);

	throw IllegalStateException("missing sql loader");
}

size_t PocoAbstractDao::execute(Statement &sql)
{
	size_t result;

	try {
		result = sql.execute();

		if (m_logger.debug()) {
			string prefix("[result: ");
			prefix += to_string(result) + "] ";
			m_logger.debug(prefix + sql.toString(),
					__FILE__, __LINE__);
		}

		return result;
	}
	catch (const Exception &e) {
		m_logger.log(e, __FILE__, __LINE__);
		m_logger.error(sql.toString(), __FILE__, __LINE__);
		log_backtrace(m_logger);
		throw;
	}
}

void PocoAbstractDao::throwMissingId(const type_info &t)
{
	log_backtrace(m_logger);
	throw InvalidArgumentException("missing id for "
			+ BeeeOn::classDemangle(t.name()));
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
