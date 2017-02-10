#ifndef BEEEON_STOPPABLE_RUNNABLE_H
#define BEEEON_STOPPABLE_RUNNABLE_H

#include <Poco/Runnable.h>

namespace BeeeOn {

class StoppableRunnable : public Poco::Runnable {
public:
	virtual ~StoppableRunnable();

	/**
	 * Stop the runnable. The call should not block.
	 */
	virtual void stop() = 0;
};

}

#endif
