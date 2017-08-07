#ifndef BEEEON_GATEWAY_ACCESS_POLICY_H
#define BEEEON_GATEWAY_ACCESS_POLICY_H

#include <Poco/SharedPtr.h>

#include "model/User.h"
#include "model/Gateway.h"

namespace BeeeOn {

class PolicyContext;

class GatewayAccessPolicy {
public:
	typedef Poco::SharedPtr<GatewayAccessPolicy> Ptr;

	virtual ~GatewayAccessPolicy();

	virtual void assureRegister(
			const PolicyContext &context,
			const Gateway &gateway) = 0;
	virtual void assureGet(
			const PolicyContext &context,
			const Gateway &gateway) = 0;
	virtual void assureUnregister(
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

