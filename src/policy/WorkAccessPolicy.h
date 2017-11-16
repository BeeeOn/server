#ifndef BEEEON_WORK_ACCESS_POLICY_H
#define BEEEON_WORK_ACCESS_POLICY_H

#include <Poco/SharedPtr.h>

namespace BeeeOn {

class PolicyContext;
class Work;

class WorkAccessPolicy {
public:
	typedef Poco::SharedPtr<WorkAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
		ACTION_USER_SCHEDULE,
		ACTION_USER_WAKEUP,
		ACTION_USER_CANCEL,
		ACTION_USER_REMOVE,
	};

	virtual ~WorkAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Work &work) = 0;
};

}

#endif
