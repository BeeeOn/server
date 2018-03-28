#include <Poco/Logger.h>
#include <Poco/Exception.h>
#include <Poco/FileStream.h>
#include <Poco/StreamCopier.h>
#include <Poco/NumberParser.h>
#include <Poco/StringTokenizer.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>

#include "di/Injectable.h"
#include "dao/poco/PocoDaoManager.h"
#include "dao/ConnectorLoader.h"
#include "util/Occasionally.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoDaoManager)
BEEEON_OBJECT_PROPERTY("connector", &PocoDaoManager::setConnector)
BEEEON_OBJECT_PROPERTY("connectionString", &PocoDaoManager::setConnectionString)
BEEEON_OBJECT_PROPERTY("minSessions", &PocoDaoManager::setMinSessions)
BEEEON_OBJECT_PROPERTY("maxSessions", &PocoDaoManager::setMaxSessions)
BEEEON_OBJECT_PROPERTY("idleTime", &PocoDaoManager::setIdleTime)
BEEEON_OBJECT_PROPERTY("connectionTimeout", &PocoDaoManager::setConnectionTimeout)
BEEEON_OBJECT_PROPERTY("features", &PocoDaoManager::setFeatures)
BEEEON_OBJECT_PROPERTY("initializers", &PocoDaoManager::addInitializer)
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
	m_idleTime(60 * Timespan::SECONDS),
	m_connectionTimeout(5 * Timespan::SECONDS)
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

void PocoDaoManager::setIdleTime(const Timespan &time)
{
	if (time.totalSeconds() < 1) {
		throw InvalidArgumentException(
			"idleTime must be at least 1 second");
	}

	m_idleTime = time;
}

void PocoDaoManager::setConnectionTimeout(const Timespan &timeout)
{
	if (timeout >= 0 && timeout.totalSeconds() < 1) {
		throw InvalidArgumentException(
			"connectionTimeout must be negative or at least a second");
	}

	m_connectionTimeout = timeout < 0? -1 : timeout;
}

void PocoDaoManager::addInitializer(SharedPtr<PocoDBInitializer> initializer)
{
	m_initializers.push_back(initializer);
}

void PocoDaoManager::setFeatures(const std::string &features)
{
	StringTokenizer tokenizer(features, ";",
			StringTokenizer::TOK_IGNORE_EMPTY
			| StringTokenizer::TOK_TRIM);
	m_features.clear();

	for (auto feature : tokenizer) {
		StringTokenizer featureTokenizer(feature, "=",
			StringTokenizer::TOK_IGNORE_EMPTY
			| StringTokenizer::TOK_TRIM);

		if (featureTokenizer.count() != 2) {
			throw InvalidArgumentException(
					"invalid feature: " + feature);
		}

		m_features[featureTokenizer[0]] =
			NumberParser::parseBool(featureTokenizer[1]);
	}
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
			+ to_string(m_idleTime.totalSeconds()),
			__FILE__, __LINE__);

	m_pool = new SessionPool(
			m_connector->name(),
			m_connectionString,
			m_minSessions,
			m_maxSessions,
			m_idleTime.totalSeconds());

	for (auto feature : m_features)
		m_pool->setFeature(feature.first, feature.second);

	logger().notice("database pool initialized", __FILE__, __LINE__);
}

void PocoDaoManager::connectAndPrepare()
{
	initPool();

	if (m_initializers.empty()) {
		logger().debug("no initializers, skipping",
				__FILE__, __LINE__);
		return;
	}

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

	for (auto initializer : m_initializers)
		initializer->initialize(session);

	logger().information("database has been initialized successfully",
			__FILE__, __LINE__);
}

void PocoDaoManager::customizeSession(Session &session)
{
	if (m_connectionTimeout >= 0)
		session.setConnectionTimeout(m_connectionTimeout.totalSeconds());
}
