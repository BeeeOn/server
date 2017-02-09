#ifndef BEEEON_LOOP_RUNNER_H
#define BEEEON_LOOP_RUNNER_H

#include <list>

#include <Poco/Mutex.h>

#include "di/InjectorTarget.h"
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

protected:
	void stopAll(std::list<StoppableLoop *> &list);
	void injectionDone() override;

private:
	bool m_autoStart;
	Poco::FastMutex m_lock;
	std::list<StoppableLoop *> m_loops;
	std::list<StoppableLoop *> m_started;
};

}

#endif
