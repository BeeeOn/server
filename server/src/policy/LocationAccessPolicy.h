#ifndef BEEEON_LOCATION_ACCESS_POLICY_H
#define BEEEON_LOCATION_ACCESS_POLICY_H

#include <Poco/SharedPtr.h>

#include "server/Session.h"
#include "model/Gateway.h"
#include "model/Location.h"
#include "model/User.h"

namespace BeeeOn {

class PolicyContext;

class LocationAccessPolicy {
public:
	typedef Poco::SharedPtr<LocationAccessPolicy> Ptr;

	enum Action {
		ACTION_USER_GET,
		ACTION_USER_CREATE,
		ACTION_USER_UPDATE,
		ACTION_USER_REMOVE,
	};

	virtual ~LocationAccessPolicy();

	virtual void assure(
			const Action action,
			const PolicyContext &context,
			const Location &location) = 0;
	virtual void assure(
			const Action action,
			const PolicyContext &context,
			const Gateway &gateway) = 0;
};

}

#endif
