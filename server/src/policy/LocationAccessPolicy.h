#ifndef BEEEON_LOCATION_ACCESS_POLICY_H
#define BEEEON_LOCATION_ACCESS_POLICY_H

#include "server/Session.h"
#include "model/Place.h"
#include "model/Location.h"

namespace BeeeOn {

class LocationAccessPolicy {
public:
	virtual void assureGet(
			const ExpirableSession::Ptr session,
			const Location &location) = 0;
	virtual void assureCreateLocation(
			const ExpirableSession::Ptr session,
			const Place &place) = 0;
	virtual void assureUpdate(
			const ExpirableSession::Ptr session,
			const Location &location) = 0;
	virtual void assureRemove(
			const ExpirableSession::Ptr session,
			const Location &location) = 0;
};

}

#endif
