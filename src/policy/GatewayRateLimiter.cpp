#include "policy/GatewayRateLimiter.h"

using namespace std;
using namespace Poco;
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

GatewayRateLimiterCache::GatewayRateLimiterCache():
	m_keepTimeout(30 * Timespan::SECONDS)
{
}

void GatewayRateLimiterCache::setKeepTimeout(const Timespan &timeout)
{
	m_keepTimeout = timeout;
}

void GatewayRateLimiterCache::keep(GatewayRateLimiter::Ptr rateLimiter)
{
	const Clock now;
	const auto id = rateLimiter->gatewayID();

	FastMutex::ScopedLock guard(m_lock);

	eraseExpired();

	auto it = m_records.find(id);
	if (it != m_records.end())
		m_expireQueue.erase(it->second);

	auto ref = m_expireQueue.emplace(now, rateLimiter);

	auto result = m_records.emplace(id, ref);
	if (!result.second)
		result.first->second = ref;
}

GatewayRateLimiter::Ptr GatewayRateLimiterCache::findAndDrop(const GatewayID &id)
{
	FastMutex::ScopedLock guard(m_lock);

	eraseExpired();

	auto it = m_records.find(id);
	if (it != m_records.end()) {
		GatewayRateLimiter::Ptr limiter = it->second->second;

		m_expireQueue.erase(it->second);
		m_records.erase(it);

		return limiter;
	}

	return nullptr;
}

void GatewayRateLimiterCache::eraseExpired(const Clock &now)
{
	const auto end = m_expireQueue.upper_bound(now - m_keepTimeout.totalMicroseconds());
	if (end == m_expireQueue.end())
		return;

	m_expireQueue.erase(m_expireQueue.begin(), end);
}
