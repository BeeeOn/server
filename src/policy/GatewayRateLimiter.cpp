#include "policy/GatewayRateLimiter.h"

using namespace BeeeOn;

GatewayRateLimiter::GatewayRateLimiter(const GatewayID &id):
	m_id(id)
{
}

GatewayRateLimiter::~GatewayRateLimiter()
{
}

const GatewayID &GatewayRateLimiter::gatewayID() const
{
	return m_id;
}

bool GatewayRateLimiter::accept(const GWMessage::Ptr)
{
	return accept();
}

GatewayRateLimiterFactory::~GatewayRateLimiterFactory()
{
}
