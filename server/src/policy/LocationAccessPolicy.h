#ifndef BEEEON_LOCATION_ACCESS_POLICY_H
#define BEEEON_LOCATION_ACCESS_POLICY_H

#include "server/Session.h"
#include "model/Place.h"
#include "model/Location.h"
#include "model/User.h"

namespace BeeeOn {

class PolicyContext;

class LocationAccessPolicy {
public:
	virtual void assureGet(
			const PolicyContext &context,
			const Location &location) = 0;
	virtual void assureCreateLocation(
			const PolicyContext &context,
			const Place &place) = 0;
	virtual void assureUpdate(
			const PolicyContext &context,
			const Location &location) = 0;
	virtual void assureRemove(
			const PolicyContext &context,
			const Location &location) = 0;
};

}

#endif
