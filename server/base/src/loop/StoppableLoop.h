#ifndef BEEEON_STOPPABLE_LOOP_H
#define BEEEON_STOPPABLE_LOOP_H

#include <Poco/SharedPtr.h>

#include "util/Loggable.h"
#include "loop/StoppableRunnable.h"

namespace Poco {

class Thread;

}

namespace BeeeOn {

class StoppableLoop {
public:
	virtual ~StoppableLoop();

	/**
	 * Starts the loop to process in an internally managed thread.
	 */
	virtual void start() = 0;

	/**
	 * Stop the loop and wait until finished.
	 */
	virtual void stop() = 0;
};

class StoppableLoopAdapter :
	public Loggable,
	public StoppableLoop {
public:
	StoppableLoopAdapter(Poco::SharedPtr<StoppableRunnable> runnable);
	~StoppableLoopAdapter();

	void start() override;
	void stop() override;

	void setStopTimeout(long ms);

protected:
	void doStop();

private:
	long m_stopTimeout;
	Poco::SharedPtr<StoppableRunnable> m_runnable;
	Poco::Thread *m_thread;
};

}

#endif
