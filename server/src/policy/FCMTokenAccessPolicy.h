#pragma once

#include <Poco/SharedPtr.h>

#include "model/FCMToken.h"

namespace BeeeOn {

class PolicyContext;

class FCMTokenAccessPolicy {
public:
	typedef Poco::SharedPtr<FCMTokenAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_REGISTER,
		ACTION_USER_UNREGISTER,
	};

	virtual ~FCMTokenAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const FCMToken &token) = 0;
};

}
