#pragma once

#include "policy/GatewayRateLimiter.h"

namespace BeeeOn {

class NoGatewayRateLimiter : public GatewayRateLimiter {
public:
	NoGatewayRateLimiter(const GatewayID &id);

	bool accept() override;
};

class NoGatewayRateLimiterFactory : public GatewayRateLimiterFactory {
public:
	GatewayRateLimiter::Ptr create(const GatewayID &id) override;
};

}
