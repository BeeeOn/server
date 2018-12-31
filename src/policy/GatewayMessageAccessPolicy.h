#pragma once

#include <Poco/SharedPtr.h>

#include "model/User.h"
#include "model/Gateway.h"

namespace BeeeOn {

class PolicyContext;

class GatewayMessageAccessPolicy {
public:
	typedef Poco::SharedPtr<GatewayMessageAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
		ACTION_USER_REMOVE,
	};

	virtual ~GatewayMessageAccessPolicy();

	virtual void assure(
		const Action action,
		const PolicyContext &context,
		const Gateway &gateway) = 0;
};

}
