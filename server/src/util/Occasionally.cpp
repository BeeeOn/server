#include "util/Occasionally.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Occasionally::Occasionally(unsigned int max,
			const Timestamp::TimeDiff delayMax):
	m_count(0),
	m_max(max),
	m_delayMax(delayMax)
{
}

void Occasionally::execute(const function<void ()> &func)
{
	FastMutex::ScopedLock guard(m_lock);

	bool run = false;

	if (m_max > 0)
		run = run || m_count == 0;
	else if (m_delayMax > 0)
		run = run || m_lastTimestamp.isElapsed(m_delayMax);

	if (run) {
		func();
		m_lastTimestamp.update();
		m_count = 1;
	} else {
		m_count = (m_count + 1) % m_max;
	}
}
