#include <string>

#include "loop/LoopRunner.h"

using namespace std;
using namespace BeeeOn;
using namespace Poco;

BEEEON_OBJECT(LoopRunner, BeeeOn::LoopRunner)

LoopRunner::LoopRunner():
	m_autoStart(false)
{
	injector("runnables", &LoopRunner::addRunnable);
	injector("loops", &LoopRunner::addLoop);
}

LoopRunner::~LoopRunner()
{
	stop();
}

void LoopRunner::addRunnable(StoppableRunnable *runnable)
{
	addLoop(new StoppableLoopAdapter(runnable));
}

void LoopRunner::addLoop(StoppableLoop *loop)
{
	FastMutex::ScopedLock guard(m_lock);

	m_loops.push_back(loop);
}

void LoopRunner::setAutoStart(bool enable)
{
	m_autoStart = enable;
}

void LoopRunner::stop()
{
	if (m_started.empty())
		return;

	logger().notice("stopping "
			+ to_string(m_started.size())
			+ " loops",
			__FILE__, __LINE__);

	FastMutex::ScopedLock guard(m_lock);

	stopAll(m_started);
	m_started.clear();
}

void LoopRunner::stopAll(list<StoppableLoop *> &list)
{
	for (auto loop : list)
		loop->stop();
}

void LoopRunner::start()
{
	FastMutex::ScopedLock guard(m_lock);

	for (auto &loop : m_loops) {
		try {
			loop->start();
			m_started.push_back(loop);
		}
		catch (...) {
			logger().critical("failed to start loop",
					__FILE__, __LINE__);

			stopAll(m_started);
			throw;
		}
	}

	logger().notice("started " + to_string(m_started.size()) + " loops",
			__FILE__, __LINE__);
}

void LoopRunner::injectionDone()
{
	if (m_autoStart) {
		logger().information("auto-starting loops", __FILE__, __LINE__);
		start();
	}
}
