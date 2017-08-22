#ifndef BEEEON_WORK_FACADE_H
#define BEEEON_WORK_FACADE_H

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class Work;

class WorkFacade {
public:
	typedef Poco::SharedPtr<WorkFacade> Ptr;

	virtual ~WorkFacade();

	virtual void schedule(Work &work) = 0;
	virtual void wakeup(Work &work) = 0;
	virtual void cancel(Work &work) = 0;
	virtual bool fetch(Work &work) = 0;
	virtual bool remove(const Work &work) = 0;
};

}

#endif
