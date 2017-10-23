#ifndef BEEEON_WORK_FACADE_H
#define BEEEON_WORK_FACADE_H

#include <set>

#include <Poco/SharedPtr.h>

#include "work/Work.h"

namespace BeeeOn {

class PolicyContext;

class WorkFacade {
public:
	typedef Poco::SharedPtr<WorkFacade> Ptr;

	virtual ~WorkFacade();

	virtual void schedule(Work &work, const PolicyContext &context) = 0;
	virtual void wakeup(Work &work, const PolicyContext &context) = 0;
	virtual void cancel(Work &work, const PolicyContext &context) = 0;
	virtual bool fetch(Work &work, const PolicyContext &context) = 0;
	virtual void fetch(std::set<Work> &works, const PolicyContext &context) = 0;
	virtual bool remove(const Work &work, const PolicyContext &context) = 0;
};

}

#endif
