#pragma once

#include <Poco/Message.h>
#include <Poco/Mutex.h>

#include "dao/QueryProfiler.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief QueryProfiler allows to watch performing of queries.
 * It periodically logs top stats during runtime and it logs
 * all stats on exit (DI cleanup).
 */
class LoggingQueryProfiler : public QueryProfiler, Loggable {
public:
	LoggingQueryProfiler();

	void use(const std::string &key) override;

	void setTopCount(int count);

	/**
	 * Log top stats via the internal logger.
	 * @param count - number of records to report (negative means all)
	 */
	void reportTopStats(int count,
		Poco::Message::Priority prio = Poco::Message::PRIO_INFORMATION);

	/**
	 * Log top stats via the internal logger and use the topCount
	 * property to limit it.
	 */
	void reportTopStats();

	/**
	 * Log all stats via the internal logger.
	 */
	void reportStats();

private:
	int m_topCount;
	Poco::Mutex m_lock;
	std::map<std::string, uint64_t> m_stats;
};

}
