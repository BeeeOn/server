#ifndef BEEEON_POCO_DAO_MANAGER_H
#define BEEEON_POCO_DAO_MANAGER_H

#include <map>

#include <Poco/SharedPtr.h>
#include <Poco/Data/SessionPool.h>

#include "dao/poco/PocoDBInitializer.h"
#include "util/Loggable.h"

namespace BeeeOn {

class ConnectorLoader;

class PocoDaoManager : public Loggable {
public:
	PocoDaoManager();

	void setConnector(ConnectorLoader *loader);
	void setConnectionString(const std::string &conn);
	void setMinSessions(const int sessions);
	void setMaxSessions(const int sessions);
	void setIdleTime(const int seconds);
	void setConnectionTimeout(const int seconds);
	void addInitializer(Poco::SharedPtr<PocoDBInitializer> initializer);
	void setFeatures(const std::string &features);

	Poco::Data::SessionPool &pool();

	void connectAndPrepare();
	void customizeSession(Poco::Data::Session &session);

protected:
	void initPool();

private:
	ConnectorLoader *m_connector;
	std::string m_connectionString;
	int m_minSessions;
	int m_maxSessions;
	int m_idleTime;
	int m_connectionTimeout;
	std::list<Poco::SharedPtr<PocoDBInitializer>> m_initializers;
	Poco::SharedPtr<Poco::Data::SessionPool> m_pool;
	std::map<std::string, bool> m_features;
};

}

#endif
