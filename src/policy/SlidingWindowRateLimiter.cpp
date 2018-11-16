#include <Poco/Clock.h>
#include <Poco/Exception.h>
#include <Poco/Logger.h>

#include "di/Injectable.h"
#include "policy/SlidingWindowRateLimiter.h"

BEEEON_OBJECT_BEGIN(BeeeOn, SlidingWindowRateLimiterFactory)
BEEEON_OBJECT_CASTABLE(GatewayRateLimiterFactory)
BEEEON_OBJECT_PROPERTY("maxRatePerMinute", &SlidingWindowRateLimiterFactory::setMaxRatePerMinute)
BEEEON_OBJECT_PROPERTY("maxRatePerHour", &SlidingWindowRateLimiterFactory::setMaxRatePerHour)
BEEEON_OBJECT_HOOK("done", &SlidingWindowRateLimiterFactory::initialize)
BEEEON_OBJECT_END(BeeeOn, SlidingWindowRateLimiterFactory)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SlidingWindowRateLimiter::SlidingWindowRateLimiter(
		const GatewayID &id,
		size_t maxRatePerMinute,
		size_t maxRatePerHour):
	GatewayRateLimiter(id),
	m_maxRatePerMinute(maxRatePerMinute),
	m_maxRatePerHour(maxRatePerHour),
	m_overlimit(false)
{
}

bool SlidingWindowRateLimiter::accept(const GWMessage::Ptr)
{
	return doAccept(Clock().microseconds());
}

bool SlidingWindowRateLimiter::accept()
{
	return doAccept(Clock().microseconds());
}

bool SlidingWindowRateLimiter::doAccept(const Poco::Timespan &now)
{
	FastMutex::ScopedLock guard(m_lock);

	shiftWindow(now);

	if (overlimit(now)) {
		m_overlimit = true;
		return false;
	}

	if (m_overlimit && logger().notice()) {
		logger().notice(
			"gateway " + gatewayID().toString()
			+ " is not overlimit anymore"
			+ " (with " + to_string(m_window.size()) + " entries)",
			__FILE__, __LINE__);
	}

	m_overlimit = false;

	auto result = m_window.emplace(align(now), 1);
	if (!result.second)
		result.first->second += 1;

	return true;
}

Timespan SlidingWindowRateLimiter::align(const Timespan &timespan) const
{
	const int sec30 = timespan.seconds() / 30;

	return {
		timespan.days(),
		timespan.hours(),
		timespan.minutes(),
		(30 * sec30) % 60,
		0
	};
}

bool SlidingWindowRateLimiter::overlimit(const Timespan &now) const
{
	static const auto ONE_MIN = 1 * Timespan::MINUTES;
	static const auto ONE_HOUR = 1 * Timespan::HOURS;

	if (overlimitGeneric(now, ONE_MIN, m_maxRatePerMinute, "limit-per-minute"))
		return true;

	return overlimitGeneric(now, ONE_HOUR, m_maxRatePerHour, "limit-per-hour");
}

bool SlidingWindowRateLimiter::overlimitGeneric(
		const Timespan &now,
		const Timespan &range,
		const size_t maxLimit,
		const string &label) const
{
	if (maxLimit == 0)
		return false;

	const auto sum = sumRange(now - range, now);

	if (logger().debug()) {
		logger().debug(
			label + ": " + to_string(sum) + "/" + to_string(maxLimit),
			__FILE__, __LINE__);
	}

	if (sum < maxLimit)
		return false;

	if (!m_overlimit && logger().warning()) {
		logger().warning(
			"gateway " + gatewayID().toString()
			+ " is over " + label
			+ " (with " + to_string(m_window.size()) + " entries"
			+ ", " + to_string(sum) + " requests)",
			__FILE__, __LINE__);
	}

	return true;
}

size_t SlidingWindowRateLimiter::sumRange(
		const Timespan &from,
		const Timespan &to) const
{
	const auto end = m_window.upper_bound(to);
	size_t sum = 0;

	for (auto it = m_window.lower_bound(from); it != end; ++it)
		sum += it->second;

	return sum;
}

void SlidingWindowRateLimiter::shiftWindow(const Timespan &now)
{
	poco_assert_msg(
		begin(m_window) == end(m_window)
		|| now >= begin(m_window)->first,
		"seems like time is not monotonic");

	const auto upperBound = m_window.upper_bound(now - 1 * Timespan::HOURS);

	if (logger().debug()) {
		const auto dist = distance(begin(m_window), upperBound);

		logger().debug(
			"shifting window by " + to_string(dist)
			+ " of total " + to_string(m_window.size()),
			__FILE__, __LINE__);
	}

	m_window.erase(begin(m_window), upperBound);
}

SlidingWindowRateLimiterFactory::SlidingWindowRateLimiterFactory():
	m_maxRatePerMinute(0),
	m_maxRatePerHour(0)
{
}

void SlidingWindowRateLimiterFactory::setMaxRatePerMinute(const int rate)
{
	if (rate < 0)
		throw InvalidArgumentException("maxRatePerMinute must not be negative");


	m_maxRatePerMinute = rate;
}

void SlidingWindowRateLimiterFactory::setMaxRatePerHour(const int rate)
{
	if (rate < 0)
		throw InvalidArgumentException("maxRatePerHour must not be negative");

	m_maxRatePerHour = rate;
}

GatewayRateLimiter::Ptr SlidingWindowRateLimiterFactory::create(const GatewayID &id)
{
	return new SlidingWindowRateLimiter(
			id,
			m_maxRatePerMinute,
			m_maxRatePerHour);
}

void SlidingWindowRateLimiterFactory::initialize()
{
	logger().information(
		"initializing sliding-window rate limiting of gateways to "
		+ to_string(m_maxRatePerMinute) + " req/min"
		+ ", " + to_string(m_maxRatePerHour) + " req/h",
		__FILE__, __LINE__);

	if (m_maxRatePerMinute == 0) {
		logger().warning(
			"maxRatePerMinute limiting is turned off",
			__FILE__, __LINE__);
	}

	if (m_maxRatePerHour == 0) {
		logger().warning(
			"maxRatePerHour limiting is turned off",
			__FILE__, __LINE__);
	}
}
