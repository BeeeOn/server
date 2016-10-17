#ifndef BEEEON_GATEWAY_ACCESS_POLICY_H
#define BEEEON_GATEWAY_ACCESS_POLICY_H

#include "model/User.h"
#include "model/Place.h"
#include "model/Gateway.h"

namespace BeeeOn {

class PolicyContext;

class GatewayAccessPolicy {
public:
	virtual void assureGet(
			const PolicyContext &context,
			const Gateway &gateway) = 0;
	virtual void assureAssignGateway(
			const PolicyContext &context,
			const Place &place) = 0;
	virtual void assureUnassign(
			const PolicyContext &context,
			const Gateway &gateway) = 0;
	virtual void assureUpdate(
			const PolicyContext &context,
			const Gateway &gateway) = 0;
	virtual void assureScanDevices(
			const PolicyContext &context,
			const Gateway &gateway) = 0;
};

}

#endif

