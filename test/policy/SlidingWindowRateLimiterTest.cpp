#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "policy/SlidingWindowRateLimiter.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class SlidingWindowRateLimiterTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SlidingWindowRateLimiterTest);
	CPPUNIT_TEST(testUnlimited);
	CPPUNIT_TEST(testLimitPerMinute);
	CPPUNIT_TEST(testLimitPerHour);
	CPPUNIT_TEST(testLimitPerMinAndHour);
	CPPUNIT_TEST_SUITE_END();
public:
	void testUnlimited();
	void testLimitPerMinute();
	void testLimitPerHour();
	void testLimitPerMinAndHour();
};

CPPUNIT_TEST_SUITE_REGISTRATION(SlidingWindowRateLimiterTest);

class TestableSlidingWindowRateLimiter : public SlidingWindowRateLimiter {
public:
	using SlidingWindowRateLimiter::doAccept;

	TestableSlidingWindowRateLimiter(
			size_t maxPerMinute,
			size_t maxPerHour):
		SlidingWindowRateLimiter(GatewayID::random(), maxPerMinute, maxPerHour)
	{
	}
};

/**
 * @brief Test (nearly) all messages are accepted when both limit checks
 * are turned off.
 */
void SlidingWindowRateLimiterTest::testUnlimited()
{
	TestableSlidingWindowRateLimiter limiter(0, 0);

	CPPUNIT_ASSERT(limiter.doAccept(0 * Timespan::SECONDS));
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::SECONDS));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::SECONDS));
	CPPUNIT_ASSERT(limiter.doAccept(3 * Timespan::SECONDS));

	for (Timespan::TimeDiff i = 4 * Timespan::SECONDS;
			i < 1 * Timespan::HOURS;
			i += 1 * Timespan::SECONDS)
		CPPUNIT_ASSERT(limiter.doAccept(i));
}

/**
 * @brief Test limiting of message rate to max 5 msg/min. Everytime
 * the limit is reached, no more messages are accepted until the
 * sliding window is moved (time advances). When the sliding window
 * is moved, messages are accepted again.
 */
void SlidingWindowRateLimiterTest::testLimitPerMinute()
{
	TestableSlidingWindowRateLimiter limiter(5, 0);

	CPPUNIT_ASSERT(limiter.doAccept(0 * Timespan::SECONDS));
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::SECONDS));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::SECONDS));
	CPPUNIT_ASSERT(limiter.doAccept(3 * Timespan::SECONDS));
	CPPUNIT_ASSERT(limiter.doAccept(4 * Timespan::SECONDS));
	CPPUNIT_ASSERT(!limiter.doAccept(5 * Timespan::SECONDS));

	for (Timespan::TimeDiff i = 6 * Timespan::SECONDS;
			i < 1 * Timespan::MINUTES;
			i += 1 * Timespan::SECONDS)
		CPPUNIT_ASSERT(!limiter.doAccept(i));

	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::MINUTES + 1));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::MINUTES + 2));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::MINUTES + 3));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::MINUTES + 4));
	CPPUNIT_ASSERT(!limiter.doAccept(2 * Timespan::MINUTES + 5));
}

/**
 * @brief Test limiting of message rate to max 5 msg/hour. Everytime
 * the limit is reached, no more messages are accepted until the
 * sliding window is moved (time advances). When the sliding window
 * is moved, messages are accepted again.
 */
void SlidingWindowRateLimiterTest::testLimitPerHour()
{
	TestableSlidingWindowRateLimiter limiter(0, 5);

	CPPUNIT_ASSERT(limiter.doAccept(0 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(3 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(4 * Timespan::MINUTES));
	CPPUNIT_ASSERT(!limiter.doAccept(5 * Timespan::MINUTES));

	for (Timespan::TimeDiff i = 6 * Timespan::MINUTES;
			i < 1 * Timespan::HOURS;
			i += 1 * Timespan::MINUTES)
		CPPUNIT_ASSERT(!limiter.doAccept(i));

	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::HOURS));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::HOURS + 1));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::HOURS + 2));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::HOURS + 3));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::HOURS + 4));
	CPPUNIT_ASSERT(!limiter.doAccept(2 * Timespan::HOURS + 5));
}

/**
 * @brief Test combination of using both rate limits set to
 * 3 msg/min, 10 msg/hour.
 */
void SlidingWindowRateLimiterTest::testLimitPerMinAndHour()
{
	TestableSlidingWindowRateLimiter limiter(3, 10);

	CPPUNIT_ASSERT(limiter.doAccept(0 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(2 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(3 * Timespan::MINUTES));

	CPPUNIT_ASSERT(limiter.doAccept(4 * Timespan::MINUTES + 1));

	// hard minute...
	CPPUNIT_ASSERT(limiter.doAccept(4 * Timespan::MINUTES + 2));
	CPPUNIT_ASSERT(limiter.doAccept(4 * Timespan::MINUTES + 3));

	// hour rate is not affected by the following (refusing) calls
	CPPUNIT_ASSERT(!limiter.doAccept(4 * Timespan::MINUTES + 4));
	CPPUNIT_ASSERT(!limiter.doAccept(4 * Timespan::MINUTES + 5));
	CPPUNIT_ASSERT(!limiter.doAccept(4 * Timespan::MINUTES + 6));

	// now we can send again
	CPPUNIT_ASSERT(limiter.doAccept(6 * Timespan::MINUTES));

	// another hard minute, but hour limit is applied first...
	CPPUNIT_ASSERT(limiter.doAccept(7 * Timespan::MINUTES + 1));
	CPPUNIT_ASSERT(limiter.doAccept(7 * Timespan::MINUTES + 2));
	CPPUNIT_ASSERT(!limiter.doAccept(7 * Timespan::MINUTES + 3));
	CPPUNIT_ASSERT(!limiter.doAccept(7 * Timespan::MINUTES + 4));

	// nearly allowed by hour limit again
	CPPUNIT_ASSERT(!limiter.doAccept(1 * Timespan::HOURS - 1));

	// new 1 allowed per hour, consume it
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::HOURS));
	CPPUNIT_ASSERT(!limiter.doAccept(1 * Timespan::HOURS + 1));

	// we are getting 3 requests from 3 shifted minutes per hour...
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::HOURS + 3 * Timespan::MINUTES));
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::HOURS + 3 * Timespan::MINUTES + 1));
	CPPUNIT_ASSERT(limiter.doAccept(1 * Timespan::HOURS + 3 * Timespan::MINUTES + 2));
	CPPUNIT_ASSERT(!limiter.doAccept(1 * Timespan::HOURS + 3 * Timespan::MINUTES + 3));
}

}
