#include <Poco/Exception.h>

#include "dao/poco/PocoAbstractDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "Debug.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace BeeeOn;

PocoAbstractDao::PocoAbstractDao():
	m_logger(LOGGER_CLASS(this)),
	m_manager(NULL)
{
	injector<PocoAbstractDao, PocoDaoManager>(
		"daoManager",
		&PocoAbstractDao::setDaoManager
	);
}

PocoAbstractDao::~PocoAbstractDao()
{
}

void PocoAbstractDao::setDaoManager(PocoDaoManager *manager)
{
	m_manager = manager;
}

PocoDaoManager &PocoAbstractDao::manager()
{
	if (m_manager)
		return *m_manager;

	throw IllegalStateException("missing manager");
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
