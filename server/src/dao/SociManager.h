#ifndef BEEEON_SOCI_MANAGER_H
#define BEEEON_SOCI_MANAGER_H

#include <map>
#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <soci.h>

#include "di/InjectorTarget.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * Manager of connection via soci. It manages the connection
 * pool. Use it as a singleton for other Soci-derrived s.
 */
class SociManager : public AbstractInjectorTarget {
public:
	SociManager():
		m_logger(LOGGER_CLASS(this)),
		m_backend(""),
		m_conn(""),
		m_poolSize(1),
		m_script(""),
		m_pool(NULL)
	{
		textInjector("backend", (TextSetter)
				&SociManager::setBackend);
		textInjector("connector", (TextSetter)
				&SociManager::setConnector);
		numberInjector("poolSize", (NumberSetter)
				&SociManager::setPoolSize);
		textInjector("initScript", (TextSetter)
				&SociManager::setInitScript);
	}

	~SociManager()
	{
		if (m_pool != NULL)
			delete m_pool;
	}

	/**
	 * Set the target backend (sqlite3, postgresql, ...).
	 */
	void setBackend(const std::string &backend)
	{
		m_backend = backend;
	}

	/**
	 * Connection string. Backend specific.
	 */
	void setConnector(const std::string conn)
	{
		m_conn = conn;
	}

	void setPoolSize(int poolSize)
	{
		if (poolSize < 1) {
			throw Poco::InvalidArgumentException(
				"poolSize must be greater then 0");
		}

		m_poolSize = poolSize;
	}

	/**
	 * Initialization SQL script file path.
	 */
	void setInitScript(const std::string &script)
	{
		m_script = script;
	}

	unsigned int poolSize() const
	{
		return m_poolSize;
	}

	soci::connection_pool &pool();

protected:
	/**
	 * After the SociManager is set up, try to initialize
	 * database if there is an initialization script specified.
	 */
	void injectionDone();

private:
	Poco::Logger &m_logger;
	std::string m_backend;
	std::string m_conn;
	unsigned int m_poolSize;
	std::string m_script;
	soci::connection_pool *m_pool;
};

}

#endif
