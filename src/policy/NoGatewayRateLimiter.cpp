#include "di/Injectable.h"
#include "policy/NoGatewayRateLimiter.h"

BEEEON_OBJECT_BEGIN(BeeeOn, NoGatewayRateLimiterFactory)
BEEEON_OBJECT_CASTABLE(GatewayRateLimiterFactory)
BEEEON_OBJECT_END(BeeeOn, NoGatewayRateLimiterFactory)

using namespace BeeeOn;

NoGatewayRateLimiter::NoGatewayRateLimiter(const GatewayID &id):
	GatewayRateLimiter(id)
{
}

bool NoGatewayRateLimiter::accept()
{
	return true;
}

GatewayRateLimiter::Ptr NoGatewayRateLimiterFactory::create(
		const GatewayID &id)
{
	return new NoGatewayRateLimiter(id);
}
