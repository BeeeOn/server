#include <Poco/Logger.h>
#include <Poco/Exception.h>
#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>

#include "dao/PocoDaoManager.h"
#include "dao/ConnectorLoader.h"
#include "dao/SQLDialect.h"
#include "util/Template.h"
#include "util/Occasionally.h"
#include "Debug.h"

BEEEON_OBJECT(PocoDaoManager, BeeeOn::PocoDaoManager)

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

PocoDaoManager::PocoDaoManager():
	m_logger(LOGGER_CLASS(this)),
	m_connector(&ConnectorLoader::null()),
	m_minSessions(1),
	m_maxSessions(32),
	m_idleTime(60),
	m_dialect(NULL),
	m_pool(NULL)
{
	injector<PocoDaoManager, ConnectorLoader>("connector",
			&PocoDaoManager::setConnector);
	textInjector("connectionString", (TextSetter)
			&PocoDaoManager::setConnectionString);
	numberInjector("minSessions", (NumberSetter)
			&PocoDaoManager::setMinSessions);
	numberInjector("maxSessions", (NumberSetter)
			&PocoDaoManager::setMaxSessions);
	numberInjector("idleTime", (NumberSetter)
			&PocoDaoManager::setIdleTime);
	textInjector("initScript", (TextSetter)
			&PocoDaoManager::setInitScript);
	injector<PocoDaoManager, SQLDialect>("dialect",
			&PocoDaoManager::setDialect);
}

void PocoDaoManager::setConnector(ConnectorLoader *connector)
{
	m_connector = connector == NULL?
		&ConnectorLoader::null() : connector;
}

void PocoDaoManager::setConnectionString(const std::string &conn)
{
	m_connectionString = conn;
}

void PocoDaoManager::setMinSessions(const int sessions)
{
	if (sessions < 1) {
		throw InvalidArgumentException(
				"minSessions must be positive");
	}

	m_minSessions = sessions;
}

void PocoDaoManager::setMaxSessions(const int sessions)
{
	if (sessions < 1) {
		throw InvalidArgumentException(
			"maxSessions must be positive");
	}

	m_maxSessions = sessions;
}

void PocoDaoManager::setIdleTime(const int seconds)
{
	if (seconds < 0) {
		throw InvalidArgumentException(
			"idleTime must be non-negative");
	}

	m_idleTime = seconds;
}

void PocoDaoManager::setInitScript(const std::string &script)
{
	m_script = script;
}

void PocoDaoManager::setDialect(SQLDialect *dialect)
{
	m_dialect = dialect;
}

Poco::Data::SessionPool &PocoDaoManager::pool()
{
	static Occasionally occasionally;

	if (m_pool == NULL)
		initPool();

	occasionally.execute([&]() {
		m_logger.notice(
			"[%s] "
			"allocated: %d, "
			"available: %d, "
			"capacity: %d, "
			"dead: %d",
			string(m_pool->isActive()? "active" : "inactive"),
			m_pool->allocated(),
			m_pool->available(),
			m_pool->capacity(),
			m_pool->dead());
	});

	return *m_pool;
}

void PocoDaoManager::initPool()
{
	if (m_minSessions > m_maxSessions)
		throw InvalidArgumentException("minSessions > maxSessions");

	m_logger.notice("initialize database pool ("
			+ m_connector->name() + ", "
			+ to_string(m_maxSessions) + ")",
			__FILE__, __LINE__);

	m_logger.debug("connection: "
			+ m_connector->name() + ", "
			+ to_string(m_minSessions) + ", "
			+ to_string(m_maxSessions) + ", "
			+ to_string(m_idleTime),
			__FILE__, __LINE__);

	m_pool = new SessionPool(
			m_connector->name(),
			m_connectionString,
			m_minSessions,
			m_maxSessions,
			m_idleTime);

	m_logger.notice("database pool initialized", __FILE__, __LINE__);
}

void PocoDaoManager::injectionDone()
{
	initPool();

	if (m_script.empty()) {
		m_logger.debug("no initialization script, skipping",
				__FILE__, __LINE__);
		return;
	}

	m_logger.debug("executing initialization script: " + m_script,
			__FILE__, __LINE__);

	FileInputStream inputStream(m_script);
	Template script(inputStream);
	map<string, string> context;

	if (!m_dialect) {
		m_logger.warning("SQL dialect is not set",
				__FILE__, __LINE__);
	} else {
		m_logger.notice("apply SQL dialect: " + m_dialect->name());
		m_dialect->specifics(context);
	}

	const string &text = script.apply(context);
	m_logger.trace(text, __FILE__, __LINE__);

	Session session(pool().get());

	m_logger.notice("Session %s transact (isolation: %u) "
			"login timeout: %z, "
			"connection timeout: %z",
			string(session.canTransact()? "can" : "cannot"),
			(unsigned int) session.getTransactionIsolation(),
			session.getLoginTimeout(),
			session.getConnectionTimeout());

	if (!session.canTransact()) {
		m_logger.warning("Session cannot perform transactions",
				__FILE__, __LINE__);
	}

	session.begin();
	session << (text), now;
	session.commit();

	m_logger.information("database has been initialized successfully",
			__FILE__, __LINE__);
}
