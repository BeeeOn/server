#ifndef BEEEON_LOOP_RUNNER_H
#define BEEEON_LOOP_RUNNER_H

#include <list>

#include <Poco/Mutex.h>

#include "di/AbstractInjectorTarget.h"
#include "loop/StoppableRunnable.h"
#include "loop/StoppableLoop.h"
#include "util/Loggable.h"

namespace BeeeOn {

class LoopRunner :
	public Loggable,
	public AbstractInjectorTarget {
public:
	LoopRunner();
	~LoopRunner();

	void addRunnable(StoppableRunnable *runnable);
	void addLoop(StoppableLoop *loop);
	void setAutoStart(bool enable);

	void start();
	void stop();
	void autoStart();

protected:
	void stopAll(std::list<StoppableLoop *> &list);

private:
	bool m_autoStart;
	Poco::FastMutex m_lock;
	std::list<StoppableLoop *> m_loops;
	std::list<StoppableLoop *> m_started;
};

}

#endif
