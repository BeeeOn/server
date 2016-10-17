#ifndef BEEEON_PLACE_ACCESS_POLICY_H
#define BEEEON_PLACE_ACCESS_POLICY_H

#include "server/Session.h"
#include "model/Place.h"
#include "model/User.h"

namespace BeeeOn {

class PolicyContext;

class PlaceAccessPolicy {
public:
	virtual void assureGet(
			const PolicyContext &context,
			const Place &place) = 0;
	virtual void assureUpdate(
			const PolicyContext &context,
			const Place &place) = 0;
	virtual void assureRemove(
			const PolicyContext &context,
			const Place &place) = 0;
};

class NullPlaceAccessPolicy : public PlaceAccessPolicy {
public:
	void assureGet(
		const PolicyContext &context,
		const Place &place) override;
	void assureUpdate(
		const PolicyContext &context,
		const Place &place) override;
	void assureRemove(
		const PolicyContext &context,
		const Place &place) override;

	static PlaceAccessPolicy &instance();
};

}

#endif

