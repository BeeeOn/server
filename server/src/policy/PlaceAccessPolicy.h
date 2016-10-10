#ifndef BEEEON_PLACE_ACCESS_POLICY_H
#define BEEEON_PLACE_ACCESS_POLICY_H

#include "server/Session.h"
#include "model/Place.h"
#include "model/User.h"

namespace BeeeOn {

class PlaceAccessPolicy {
public:
	virtual void assureGet(
			const User &user,
			const Place &place) = 0;
	virtual void assureUpdate(
			const User &user,
			const Place &place) = 0;
	virtual void assureRemove(
			const User &user,
			const Place &place) = 0;
};

}

#endif

