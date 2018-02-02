#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "dao/LoggingQueryProfiler.h"
#include "util/Occasionally.h"

BEEEON_OBJECT_BEGIN(BeeeOn, LoggingQueryProfiler)
BEEEON_OBJECT_CASTABLE(QueryProfiler)
BEEEON_OBJECT_NUMBER("topCount", &LoggingQueryProfiler::setTopCount)
BEEEON_OBJECT_HOOK("cleanup", &LoggingQueryProfiler::reportStats)
BEEEON_OBJECT_END(BeeeOn, LoggingQueryProfiler)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

LoggingQueryProfiler::LoggingQueryProfiler():
	m_topCount(10)
{
}

void LoggingQueryProfiler::setTopCount(int count)
{
	if (count < 0) {
		throw InvalidArgumentException(
			"topCount must be non-negative");
	}

	m_topCount = count;
}

void LoggingQueryProfiler::use(const string &key)
{
	static Occasionally occasionally;

	Mutex::ScopedLock guard(m_lock);

	auto result = m_stats.emplace(key, 1);
	if (!result.second)
		result.first->second += 1;

	occasionally.execute([&]() {
		reportTopStats();
	});
}

void LoggingQueryProfiler::reportTopStats(int count, Message::Priority prio)
{
	if (count == 0)
		return;

	multimap<uint64_t, string> sorted;

	ScopedLockWithUnlock<Mutex> guard(m_lock);
	for (const auto &pair : m_stats)
		sorted.emplace(pair.second, pair.first);

	guard.unlock();
	string record;

	record += "queries usage stats:";

	for (auto it = sorted.rbegin(); it != sorted.rend(); ++it) {
		if (count >= 0 && count-- == 0)
			break;

		record += "\n";
		record += it->second + ": " + to_string(it->first);
	}

	logger().log(Message{
		logger().name(), record, prio, __FILE__, __LINE__});
}

void LoggingQueryProfiler::reportTopStats()
{
	reportTopStats(m_topCount, Message::PRIO_INFORMATION);
}

void LoggingQueryProfiler::reportStats()
{
	reportTopStats(-1, Message::PRIO_NOTICE);
}
