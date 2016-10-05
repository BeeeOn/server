#ifndef BEEEON_PLACE_ACCESS_POLICY_H
#define BEEEON_PLACE_ACCESS_POLICY_H

#include "server/Session.h"
#include "model/Place.h"

namespace BeeeOn {

class PlaceAccessPolicy {
public:
	virtual void assureGet(
			const ExpirableSession::Ptr session,
			const Place &place) = 0;
	virtual void assureUpdate(
			const ExpirableSession::Ptr session,
			const Place &place) = 0;
	virtual void assureRemove(
			const ExpirableSession::Ptr session,
			const Place &place) = 0;
};

}

#endif

