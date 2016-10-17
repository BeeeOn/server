#ifndef BEEEON_POCO_DAO_MANAGER_H
#define BEEEON_POCO_DAO_MANAGER_H

#include "di/InjectorTarget.h"

namespace Poco {

class Logger;

namespace Data {

class SessionPool;

}
}

namespace BeeeOn {

class SQLDialect;

class PocoDaoManager : public AbstractInjectorTarget {
public:
	PocoDaoManager();

	void setConnector(const std::string &connector);
	void setConnectionString(const std::string &conn);
	void setMinSessions(const int sessions);
	void setMaxSessions(const int sessions);
	void setIdleTime(const int seconds);
	void setInitScript(const std::string &script);
	void setDialect(SQLDialect *dialect);

	Poco::Data::SessionPool &pool();

protected:
	void initPool();
	void injectionDone() override;

private:
	Poco::Logger &m_logger;
	std::string m_connector;
	std::string m_connectionString;
	int m_minSessions;
	int m_maxSessions;
	int m_idleTime;
	std::string m_script;
	SQLDialect *m_dialect;
	Poco::Data::SessionPool *m_pool;
};

}

#endif
