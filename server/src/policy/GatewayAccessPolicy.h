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

class NullGatewayAccessPolicy : public GatewayAccessPolicy {
public:
	void assureGet(
		const PolicyContext &context,
		const Gateway &gateway) override;
	void assureUnassign(
		const PolicyContext &context,
		const Gateway &gateway) override;
	void assureUpdate(
		const PolicyContext &context,
		const Gateway &gateway) override;
	void assureScanDevices(
		const PolicyContext &context,
		const Gateway &gateway) override;

	static GatewayAccessPolicy &instance();
};

}

#endif

