#include <Poco/Logger.h>
#include <Poco/Exception.h>
#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>

#include "di/Injectable.h"
#include "dao/poco/PocoDaoManager.h"
#include "dao/ConnectorLoader.h"
#include "util/Template.h"
#include "util/Occasionally.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoDaoManager)
BEEEON_OBJECT_REF("connector", &PocoDaoManager::setConnector)
BEEEON_OBJECT_TEXT("connectionString", &PocoDaoManager::setConnectionString)
BEEEON_OBJECT_NUMBER("minSessions", &PocoDaoManager::setMinSessions)
BEEEON_OBJECT_NUMBER("maxSessions", &PocoDaoManager::setMaxSessions)
BEEEON_OBJECT_NUMBER("idleTime", &PocoDaoManager::setIdleTime)
BEEEON_OBJECT_TEXT("initScript", &PocoDaoManager::setInitScript)
BEEEON_OBJECT_HOOK("done", &PocoDaoManager::connectAndPrepare)
BEEEON_OBJECT_END(BeeeOn, PocoDaoManager)

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

PocoDaoManager::PocoDaoManager():
	m_connector(&ConnectorLoader::null()),
	m_minSessions(1),
	m_maxSessions(32),
	m_idleTime(60)
{
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

Poco::Data::SessionPool &PocoDaoManager::pool()
{
	static Occasionally occasionally;

	if (m_pool.isNull())
		initPool();

	occasionally.execute([&]() {
		logger().notice(
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

	logger().notice("initialize database pool ("
			+ m_connector->name() + ", "
			+ to_string(m_maxSessions) + ")",
			__FILE__, __LINE__);

	logger().debug("connection: "
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

	logger().notice("database pool initialized", __FILE__, __LINE__);
}

void PocoDaoManager::connectAndPrepare()
{
	initPool();

	if (m_script.empty()) {
		logger().debug("no initialization script, skipping",
				__FILE__, __LINE__);
		return;
	}

	logger().debug("executing initialization script: " + m_script,
			__FILE__, __LINE__);

	FileInputStream inputStream(m_script);
	Template script(inputStream);
	map<string, string> context;

	// XXX: no context for now, this feature will be probably dropped

	const string &text = script.apply(context);
	logger().trace(text, __FILE__, __LINE__);

	Session session(pool().get());

	logger().notice("Session %s transact (isolation: 0x%04x) "
			"login timeout: %z, "
			"connection timeout: %z",
			string(session.canTransact()? "can" : "cannot"),
			(unsigned int) session.getTransactionIsolation(),
			session.getLoginTimeout(),
			session.getConnectionTimeout());

	if (!session.canTransact()) {
		logger().warning("Session cannot perform transactions",
				__FILE__, __LINE__);
	}

	session.begin();
	session << (text), now;
	session.commit();

	logger().information("database has been initialized successfully",
			__FILE__, __LINE__);
}
