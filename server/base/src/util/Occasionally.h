#ifndef BEEEON_OCCASIONALLY_H
#define BEEEON_OCCASIONALLY_H

#include <functional>
#include <Poco/Timestamp.h>
#include <Poco/Mutex.h>

namespace BeeeOn {

/**
 * Helper providing occasional execution of some command.
 * E.g. we want to log some events but not every time we
 * execute the call. This class wraps a sampling capability
 * that provides just occasional calls.
 */
class Occasionally {
public:
	Occasionally(unsigned int max = 1000,
			const Poco::Timestamp::TimeDiff delayMax = 0);

	void execute(const std::function<void ()> &func);

private:
	unsigned int m_count;
	unsigned int m_max;
	Poco::Timestamp m_lastTimestamp;
	Poco::Timestamp::TimeDiff m_delayMax;
	Poco::FastMutex m_lock;
};

}

#endif
