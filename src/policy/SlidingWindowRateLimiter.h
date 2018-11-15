#pragma once

#include <map>

#include <Poco/Mutex.h>
#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "policy/GatewayRateLimiter.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief SlidingWindowRateLimiter keeps track of a recent amount of messages
 * received from a certain gateway. The amount of messages is defined by
 * the size of the sliding window which is currently fixed to 1 hour.
 * The SlidingWindowRateLimiter supports policing of messages based of
 * traffic per minute and/or traffic per hour.
 *
 * Principle:
 *
 * - sliding window is divided into slots of size 30 seconds
 *
 * - for each slot, the number of seen messages is measured
 *
 * - when for the most recent minute (usually 2 slots) the measured
 *   number of messages reaches the max-rate-per-minute limit, no more
 *   messages are accepted until the sliding window is moved enough
 *
 * - when for the most recent hour (usually 120 slots) the measured
 *   number of measured reaches the max-rate-per-hour limit, no more
 *   messages are accepted until the sliding window is moved enough
 *
 * - if the max-rate-per-minute starts dropping, the max-rate-per-hour
 *   is left intact and thus it is not being exhausted by high message
 *   rate during a single minute
 *
 * Example:
 *
 * - consider max rates: 20 msg/min (M), 70 msg/h (H)
 *
 * <pre>
 * slot: 0    30   60   90  120  150  180  210  240  270  300 ... 3600
 * cnt:  |  0 | 15 |  5 |  8 | 12 |  4 | 16 |  4 |  6 |  0 |  ... | 5 |
 *                    M         M         M    M    H    H
 * </pre>
 */
class SlidingWindowRateLimiter :
	Loggable,
	public GatewayRateLimiter {
public:
	typedef Poco::SharedPtr<SlidingWindowRateLimiter> Ptr;

	SlidingWindowRateLimiter(
		const GatewayID &id,
		size_t maxRatePerMinute,
		size_t maxRatePerHour);

	/**
	 * @brief Accepting of all messages uses the same algorithm.
	 * The call is thread-safe.
	 *
	 * @see SlidingWindowRateLimiter::doAccept()
	 */
	bool accept(const GWMessage::Ptr msg) override;

	/**
	 * @brief Accepting of all messages uses the same algorithm.
	 * The call is thread-safe.
	 *
	 * @see SlidingWindowRateLimiter::doAccept()
	 */
	bool accept() override;

protected:
	/**
	 * @brief Move the sliding window according to the current time
	 * represented as Poco::Timespan and check whether the current
	 * message fits into all limits.
	 */
	bool doAccept(const Poco::Timespan &now);

	/**
	 * @brief Align the given time to multiples of 30 seconds.
	 */
	Poco::Timespan align(const Poco::Timespan &timespan) const;

	/**
	 * @brief Test overlimit of the current message at the given
	 * time point.
	 */
	bool overlimit(const Poco::Timespan &now) const;

	/**
	 * @brief Generic algorithm for any time window. It computes
	 * number of messages received in the given time range until now.
	 * If the amount is greater then the given maxLimit, it returns
	 * true.
	 *
	 * @returns true if the current message is overlimit, otherwise false.
	 */
	bool overlimitGeneric(
		const Poco::Timespan &now,
		const Poco::Timespan &range,
		const size_t maxLimit,
		const std::string &label) const;

	/**
	 * @brief Sum messages amounts in the given time interval.
	 */
	size_t sumRange(const Poco::Timespan &b, const Poco::Timespan &e) const;

	/**
	 * @brief Remove old records from the sliding window. Records older
	 * then 1 hour are simply erased.
	 */
	void shiftWindow(const Poco::Timespan &now);

private:
	const size_t m_maxRatePerMinute;
	const size_t m_maxRatePerHour;
	bool m_overlimit;
	std::map<Poco::Timespan, size_t> m_window;
	mutable Poco::FastMutex m_lock;
};

/**
 * @brief Factory to create SlidingWindowRateLimiter instances
 * with the shared configuration.
 */
class SlidingWindowRateLimiterFactory :
	Loggable,
	public GatewayRateLimiterFactory {
public:
	SlidingWindowRateLimiterFactory();

	/**
	 * @brief Configure max-rate-per-minute. The value 0 would
	 * turn this limit off (it would be ignored).
	 */
	void setMaxRatePerMinute(const int rate);

	/**
	 * @brief Configure max-rate-per-hour. The value 0 would
	 * turn this limit off (it would be ignored).
	 */
	void setMaxRatePerHour(const int rate);

	/**
	 * @returns new instance of SlidingWindowRateLimiter.
	 */
	GatewayRateLimiter::Ptr create(const GatewayID &id) override;

	/**
	 * @brief Log and validate configuration.
	 */
	void initialize();

private:
	size_t m_maxRatePerMinute;
	size_t m_maxRatePerHour;
};

}
