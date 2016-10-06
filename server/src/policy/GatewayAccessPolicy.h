#ifndef BEEEON_GATEWAY_ACCESS_POLICY_H
#define BEEEON_GATEWAY_ACCESS_POLICY_H

#include "server/Session.h"
#include "model/Place.h"
#include "model/Gateway.h"

namespace BeeeOn {

class GatewayAccessPolicy {
public:
	virtual void assureGet(
			const ExpirableSession::Ptr session,
			const Gateway &gateway) = 0;
	virtual void assureAssignGateway(
			const ExpirableSession::Ptr session,
			const Place &place) = 0;
	virtual void assureUnassign(
			const ExpirableSession::Ptr session,
			const Gateway &gateway) = 0;
	virtual void assureUpdate(
			const ExpirableSession::Ptr session,
			const Gateway &gateway) = 0;
	virtual void assureScanDevices(
			const ExpirableSession::Ptr session,
			const Gateway &gateway) = 0;
};

}

#endif

