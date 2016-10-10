#ifndef BEEEON_GATEWAY_ACCESS_POLICY_H
#define BEEEON_GATEWAY_ACCESS_POLICY_H

#include "model/User.h"
#include "model/Place.h"
#include "model/Gateway.h"

namespace BeeeOn {

class GatewayAccessPolicy {
public:
	virtual void assureGet(
			const User &user,
			const Gateway &gateway) = 0;
	virtual void assureAssignGateway(
			const User &user,
			const Place &place) = 0;
	virtual void assureUnassign(
			const User &user,
			const Gateway &gateway) = 0;
	virtual void assureUpdate(
			const User &user,
			const Gateway &gateway) = 0;
	virtual void assureScanDevices(
			const User &user,
			const Gateway &gateway) = 0;
};

}

#endif

