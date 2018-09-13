#pragma once

#include <Poco/SharedPtr.h>

#include "model/User.h"
#include "model/Gateway.h"

namespace BeeeOn {

class PolicyContext;

class GatewayAccessPolicy {
public:
	typedef Poco::SharedPtr<GatewayAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_REGISTER,
		ACTION_USER_UNREGISTER,
		ACTION_USER_GET,
		ACTION_USER_UPDATE,
		ACTION_USER_SCAN,
	};

	virtual ~GatewayAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Gateway &gateway) = 0;
};

}
