#ifndef BEEEON_WORK_SERVICE_H
#define BEEEON_WORK_SERVICE_H

#include <list>

#include <Poco/SharedPtr.h>

#include "service/Single.h"
#include "work/Work.h"

namespace BeeeOn {

class WorkService {
public:
	typedef Poco::SharedPtr<WorkService> Ptr;

	virtual ~WorkService();

	virtual bool fetch(Single<Work> &input) = 0;
	virtual void list(Single<std::list<Work>> &input) = 0;
	virtual void cancel(Single<Work> &input) = 0;
	virtual void remove(Single<Work> &input) = 0;
};

}

#endif
