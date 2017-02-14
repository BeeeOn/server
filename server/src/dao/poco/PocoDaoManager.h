#ifndef BEEEON_POCO_DAO_MANAGER_H
#define BEEEON_POCO_DAO_MANAGER_H

#include <Poco/SharedPtr.h>
#include <Poco/Data/SessionPool.h>
#include "di/AbstractInjectorTarget.h"
#include "util/Loggable.h"

namespace BeeeOn {

class ConnectorLoader;

class PocoDaoManager : public AbstractInjectorTarget,
		public Loggable {
public:
	PocoDaoManager();

	void setConnector(ConnectorLoader *loader);
	void setConnectionString(const std::string &conn);
	void setMinSessions(const int sessions);
	void setMaxSessions(const int sessions);
	void setIdleTime(const int seconds);
	void setInitScript(const std::string &script);

	Poco::Data::SessionPool &pool();

	void connectAndPrepare();

protected:
	void initPool();

private:
	ConnectorLoader *m_connector;
	std::string m_connectionString;
	int m_minSessions;
	int m_maxSessions;
	int m_idleTime;
	std::string m_script;
	Poco::SharedPtr<Poco::Data::SessionPool> m_pool;
};

}

#endif
