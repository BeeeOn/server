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

	virtual ~LocationAccessPolicy();

	virtual void assureGet(
			const PolicyContext &context,
			const Location &location) = 0;
	virtual void assureCreateLocation(
			const PolicyContext &context,
			const Gateway &gateway) = 0;
	virtual void assureUpdate(
			const PolicyContext &context,
			const Location &location) = 0;
	virtual void assureRemove(
			const PolicyContext &context,
			const Location &location) = 0;
};

}

#endif
