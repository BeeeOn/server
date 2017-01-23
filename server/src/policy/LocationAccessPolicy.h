#ifndef BEEEON_LOCATION_ACCESS_POLICY_H
#define BEEEON_LOCATION_ACCESS_POLICY_H

#include "server/Session.h"
#include "model/Gateway.h"
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
			const Gateway &gateway) = 0;
	virtual void assureUpdate(
			const PolicyContext &context,
			const Location &location) = 0;
	virtual void assureRemove(
			const PolicyContext &context,
			const Location &location) = 0;
};

class NullLocationAccessPolicy : public LocationAccessPolicy {
public:
	void assureGet(
		const PolicyContext &context,
		const Location &location) override;
	void assureCreateLocation(
		const PolicyContext &context,
		const Gateway &gateway) override;
	void assureUpdate(
		const PolicyContext &context,
		const Location &location) override;
	void assureRemove(
		const PolicyContext &context,
		const Location &location) override;

	static LocationAccessPolicy &instance();
};

}

#endif
