#include <cppunit/extensions/HelperMacros.h>

#include "cppunit/BetterAssert.h"
#include "policy/SensorHistoryRules.h"

using namespace std;
using namespace Poco;

namespace BeeeOn {

class SensorHistoryRulesTest : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(SensorHistoryRulesTest);
	CPPUNIT_TEST(testSpanLessThan);
	CPPUNIT_TEST(testParseValidSpan);
	CPPUNIT_TEST(testParseInvalidSpan);
	CPPUNIT_TEST(testSetValidRules);
	CPPUNIT_TEST(testSetInvalidRules);
	CPPUNIT_TEST(testAccept);
	CPPUNIT_TEST(testNotAccept);
	CPPUNIT_TEST_SUITE_END();
public:
	void testSpanLessThan();
	void testParseValidSpan();
	void testParseInvalidSpan();
	void testSetValidRules();
	void testSetInvalidRules();
	void testAccept();
	void testNotAccept();

protected:
	TimeInterval minutes(unsigned int m, bool almost = false) const;
	TimeInterval hours(unsigned int h, bool almost = false) const;
	TimeInterval days(unsigned int d, bool almost = false) const;

private:
};

CPPUNIT_TEST_SUITE_REGISTRATION(SensorHistoryRulesTest);

TimeInterval SensorHistoryRulesTest::minutes(unsigned int m, bool almost) const
{
	return {0, m * Timespan::MINUTES - (almost ? 1 : 0)};
}

TimeInterval SensorHistoryRulesTest::hours(unsigned int h, bool almost) const
{
	return {0, h * Timespan::HOURS - (almost ? 1 : 0)};
}

TimeInterval SensorHistoryRulesTest::days(unsigned int d, bool almost) const
{
	return {0, d * Timespan::DAYS - (almost ? 1 : 0)};
}

void SensorHistoryRulesTest::testSpanLessThan()
{
	using Span = SensorHistoryRules::Span;

	CPPUNIT_ASSERT(Span(0, 0) < Span(1, 1));
	CPPUNIT_ASSERT(Span(0, 0) < Span(1, 2));
	CPPUNIT_ASSERT(Span(0, 1) < Span(1, 1));
	CPPUNIT_ASSERT(Span(0, 1) < Span(2, 2));

	CPPUNIT_ASSERT(!(Span(0, 0) < Span(0, 1)));
	CPPUNIT_ASSERT(!(Span(0, 0) < Span(0, 2)));
	CPPUNIT_ASSERT(!(Span(0, 1) < Span(0, 1)));
	CPPUNIT_ASSERT(!(Span(0, 1) < Span(0, 0)));

	// check representation 0..1, 1..1, 1..2
	// 0..1 and 1..1 can coexist
	CPPUNIT_ASSERT((Span(0, 1) < Span(1, 1)));
	// 1..1 and 1..2 are overlapping
	CPPUNIT_ASSERT(!(Span(1, 1) < Span(1, 2)));
}

void SensorHistoryRulesTest::testParseValidSpan()
{
	const auto a = SensorHistoryRules::parseSpan("1 s");
	CPPUNIT_ASSERT(a.min == 1 * Timespan::SECONDS);
	CPPUNIT_ASSERT(a.max == 1 * Timespan::SECONDS);

	const auto b = SensorHistoryRules::parseSpan("1 m");
	CPPUNIT_ASSERT(b.min == 1 * Timespan::MINUTES);
	CPPUNIT_ASSERT(b.max == 1 * Timespan::MINUTES);

	const auto c = SensorHistoryRules::parseSpan("1 m..1 h");
	CPPUNIT_ASSERT(c.min == 1 * Timespan::MINUTES);
	CPPUNIT_ASSERT(c.max == 1 * Timespan::HOURS);

	const auto d = SensorHistoryRules::parseSpan("1 h..2 h");
	CPPUNIT_ASSERT(d.min == 1 * Timespan::HOURS);
	CPPUNIT_ASSERT(d.max == 2 * Timespan::HOURS);

	const auto e = SensorHistoryRules::parseSpan("0 h");
	CPPUNIT_ASSERT(e.min == 0);
	CPPUNIT_ASSERT(e.max == 0);

	const auto f = SensorHistoryRules::parseSpan("15 h");
	CPPUNIT_ASSERT(f.min == 15 * Timespan::HOURS);
	CPPUNIT_ASSERT(f.max == 15 * Timespan::HOURS);

	const auto g = SensorHistoryRules::parseSpan("15 h..15 h");
	CPPUNIT_ASSERT(g.min == 15 * Timespan::HOURS);
	CPPUNIT_ASSERT(g.max == 15 * Timespan::HOURS);

	const auto h = SensorHistoryRules::parseSpan("max");
	CPPUNIT_ASSERT(h.min == Timestamp::TIMEVAL_MAX);
	CPPUNIT_ASSERT(h.max == Timestamp::TIMEVAL_MAX);

	const auto i = SensorHistoryRules::parseSpan("0..max");
	CPPUNIT_ASSERT(i.min == 0);
	CPPUNIT_ASSERT(i.max == Timestamp::TIMEVAL_MAX);

	const auto j = SensorHistoryRules::parseSpan("1 s..1 s+");
	CPPUNIT_ASSERT(j.min == 1 * Timespan::SECONDS);
	CPPUNIT_ASSERT(j.max == 1 * Timespan::SECONDS + 1);

	const auto k = SensorHistoryRules::parseSpan("1 s..1 h+");
	CPPUNIT_ASSERT(k.min == 1 * Timespan::SECONDS);
	CPPUNIT_ASSERT(k.max == 1 * Timespan::HOURS + 1);

	const auto l = SensorHistoryRules::parseSpan("1 s+..1 h");
	CPPUNIT_ASSERT(l.min == 1 * Timespan::SECONDS + 1);
	CPPUNIT_ASSERT(l.max == 1 * Timespan::HOURS);

	const auto m = SensorHistoryRules::parseSpan("1 s+..1 s+");
	CPPUNIT_ASSERT(m.min == 1 * Timespan::SECONDS + 1);
	CPPUNIT_ASSERT(m.max == 1 * Timespan::SECONDS + 1);
}

void SensorHistoryRulesTest::testParseInvalidSpan()
{
	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan("1 h..2 h.."),
		SyntaxException);

	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan(""),
		SyntaxException);

	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan("1.5 s..1.2 s"),
		SyntaxException);

	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan("..1.2 s"),
		SyntaxException);

	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan("5 s..1 s"),
		InvalidArgumentException);

	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan("max..max"),
		SyntaxException);

	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan("1 s+"),
		SyntaxException);

	CPPUNIT_ASSERT_THROW(
		SensorHistoryRules::parseSpan("1 s+..1 s"),
		InvalidArgumentException);
}

void SensorHistoryRulesTest::testSetValidRules()
{
	SensorHistoryRules rules;

	CPPUNIT_ASSERT_NO_THROW(
		rules.parseAndSetRules({
			{"0", "0"},
			{"1", "0"},
			{"2", "1"},
			{"3", "1"},
			{"4..5", "5"},
			{"5..6", "1"},
		}));
}

void SensorHistoryRulesTest::testSetInvalidRules()
{
	SensorHistoryRules rules;

	CPPUNIT_ASSERT_THROW(
		rules.parseAndSetRules({
			{"0", "0"},
			{"0 us", "0"}, // conflicting
		}),
		ExistsException);

	CPPUNIT_ASSERT_THROW(
		rules.parseAndSetRules({
			{"0", "0"},
			{"1 m", "0"},
			{"60 s", "0"}, // conflicting
		}),
		ExistsException);

	CPPUNIT_ASSERT_THROW(
		rules.parseAndSetRules({
			{"0..1 s", "0"},
			{"1 s",    "0"},
			{"1000 ms..2 s", "0"}, // conflicting
		}),
		ExistsException);

	CPPUNIT_ASSERT_THROW(
		rules.parseAndSetRules({
			{"1..0", "0"}, // min > max
		}),
		InvalidArgumentException);

	CPPUNIT_ASSERT_THROW(
		rules.parseAndSetRules({
			{"*", "0"},
			{"1", "0"},
			{"* ", "0"}, // duplicate
		}),
		ExistsException);
}

/**
 * Define a set of rules and test boundary conditions of each.
 * There are 2 kinds if tests:
 *
 * - test that the rule exists
 * - test that the interval is valid for the given rule
 *
 * The rules does not contain holes except of the 60 d.
 * Holes would be however covered by the rule "*".
 */
void SensorHistoryRulesTest::testAccept()
{
	SensorHistoryRules rules;
	rules.setAcceptZeroInterval(true);

	CPPUNIT_ASSERT_NO_THROW(
		rules.parseAndSetRules({
			{"0..10 m", "0"},
			{"10 m..1 h", "5 s..1 h"},
			{"1 h..5 h", "30 s..1 h"},
			{"5 h..10 h", "30 s..5 h"},
			{"10 h..1 d", "5 m..5 h"},
			{"1 d..7 d", "1 h..1 d"},
			{"7 d..30 d", "1 d..7 d"},
			{"60 d", "10 d"},
			{"*", "1 d..30 d"},
		}));

	// 0 is globally accepted
	CPPUNIT_ASSERT(rules.accept(minutes(0), 0));
	CPPUNIT_ASSERT(rules.accept(minutes(10, true), 0));

	// inside 5 s..1 h
	CPPUNIT_ASSERT(rules.accept(minutes(10), 5 * Timespan::SECONDS));
	CPPUNIT_ASSERT(rules.accept(minutes(10), 1 * Timespan::HOURS));
	CPPUNIT_ASSERT(rules.accept(hours(1, true), 5 * Timespan::SECONDS));
	CPPUNIT_ASSERT(rules.accept(hours(1, true), 1 * Timespan::HOURS));

	// inside 30 s..1 h
	CPPUNIT_ASSERT(rules.accept(hours(1), 30 * Timespan::SECONDS));
	CPPUNIT_ASSERT(rules.accept(hours(1), 1 * Timespan::HOURS));
	CPPUNIT_ASSERT(rules.accept(hours(5, true), 30 * Timespan::SECONDS));
	CPPUNIT_ASSERT(rules.accept(hours(5, true), 1 * Timespan::HOURS));

	// inside 5 m..5 h
	CPPUNIT_ASSERT(rules.accept(hours(10), 5 * Timespan::MINUTES));
	CPPUNIT_ASSERT(rules.accept(hours(10), 5 * Timespan::HOURS));
	CPPUNIT_ASSERT(rules.accept(hours(24, true), 5 * Timespan::MINUTES));
	CPPUNIT_ASSERT(rules.accept(hours(24, true), 5 * Timespan::HOURS));

	// inside 1 h..1 d
	CPPUNIT_ASSERT(rules.accept(days(1), 1 * Timespan::HOURS));
	CPPUNIT_ASSERT(rules.accept(days(1), 1 * Timespan::DAYS));
	CPPUNIT_ASSERT(rules.accept(days(7, true), 1 * Timespan::HOURS));
	CPPUNIT_ASSERT(rules.accept(days(7, true), 1 * Timespan::DAYS));

	// inside 1 d..7 d
	CPPUNIT_ASSERT(rules.accept(days(7), 1 * Timespan::DAYS));
	CPPUNIT_ASSERT(rules.accept(days(7), 7 * Timespan::DAYS));
	CPPUNIT_ASSERT(rules.accept(days(30, true), 1 * Timespan::DAYS));
	CPPUNIT_ASSERT(rules.accept(days(30, true), 7 * Timespan::DAYS));

	// inside 1 d..30 d
	CPPUNIT_ASSERT(rules.accept(days(30), 1 * Timespan::DAYS));
	CPPUNIT_ASSERT(rules.accept(days(30), 30 * Timespan::DAYS));
	CPPUNIT_ASSERT(rules.accept(days(100), 1 * Timespan::DAYS));
	CPPUNIT_ASSERT(rules.accept(days(100), 30 * Timespan::DAYS));

	// exactly 10 d
	CPPUNIT_ASSERT(rules.accept(days(60), 10 * Timespan::DAYS));
}

void SensorHistoryRulesTest::testNotAccept()
{
	SensorHistoryRules rules;
	rules.setAcceptZeroInterval(false);

	CPPUNIT_ASSERT_NO_THROW(
		rules.parseAndSetRules({
			{"0..10 m", "0"},
			{"10 m..1 h", "5 s..1 h"},
			{"1 h..5 h", "30 s..1 h"},
			{"5 h..10 h", "30 s..5 h"},
			{"10 h..1 d", "5 m..5 h"},
			{"1 d..7 d", "1 h..1 d"},
			{"7 d..30 d", "1 d..7 d"},
			{"60 d", "10 d"},
			{"*", "1 d..30 d"},
		}));

	// 0 interval is globally denied for those rules
	CPPUNIT_ASSERT(!rules.accept(minutes(0), 0));
	CPPUNIT_ASSERT(!rules.accept(minutes(10, true), 0));

	// out of 5 s..1 h
	CPPUNIT_ASSERT(!rules.accept(minutes(10), 5 * Timespan::SECONDS - 1));
	CPPUNIT_ASSERT(!rules.accept(minutes(10), 1 * Timespan::HOURS + 1));
	CPPUNIT_ASSERT(!rules.accept(hours(1, true), 5 * Timespan::SECONDS - 1));
	CPPUNIT_ASSERT(!rules.accept(hours(1, true), 1 * Timespan::HOURS + 1));

	// out of 30 s..1 h
	CPPUNIT_ASSERT(!rules.accept(hours(1), 30 * Timespan::SECONDS - 1));
	CPPUNIT_ASSERT(!rules.accept(hours(1), 1 * Timespan::HOURS + 1));
	CPPUNIT_ASSERT(!rules.accept(hours(5, true), 30 * Timespan::SECONDS - 1));
	CPPUNIT_ASSERT(!rules.accept(hours(5, true), 1 * Timespan::HOURS + 1));

	// out of 5 m..5 h
	CPPUNIT_ASSERT(!rules.accept(hours(10), 5 * Timespan::MINUTES - 1));
	CPPUNIT_ASSERT(!rules.accept(hours(10), 5 * Timespan::HOURS + 1));
	CPPUNIT_ASSERT(!rules.accept(hours(24, true), 5 * Timespan::MINUTES - 1));
	CPPUNIT_ASSERT(!rules.accept(hours(24, true), 5 * Timespan::HOURS + 1));

	// out of 1 h..1 d
	CPPUNIT_ASSERT(!rules.accept(days(1), 1 * Timespan::HOURS - 1));
	CPPUNIT_ASSERT(!rules.accept(days(1), 1 * Timespan::DAYS + 1));
	CPPUNIT_ASSERT(!rules.accept(days(7, true), 1 * Timespan::HOURS - 1));
	CPPUNIT_ASSERT(!rules.accept(days(7, true), 1 * Timespan::DAYS + 1));

	// out of 1 d..7 d
	CPPUNIT_ASSERT(!rules.accept(days(7), 1 * Timespan::DAYS - 1));
	CPPUNIT_ASSERT(!rules.accept(days(7), 7 * Timespan::DAYS + 1));
	CPPUNIT_ASSERT(!rules.accept(days(30, true), 1 * Timespan::DAYS - 1));
	CPPUNIT_ASSERT(!rules.accept(days(30, true), 7 * Timespan::DAYS + 1));

	// out of 1 d..30 d
	CPPUNIT_ASSERT(!rules.accept(days(30), 1 * Timespan::DAYS - 1));
	CPPUNIT_ASSERT(!rules.accept(days(30), 30 * Timespan::DAYS + 1));
	CPPUNIT_ASSERT(!rules.accept(days(100), 1 * Timespan::DAYS - 1));
	CPPUNIT_ASSERT(!rules.accept(days(100), 30 * Timespan::DAYS + 1));

	// out of 10 d
	CPPUNIT_ASSERT(!rules.accept(days(60), 10 * Timespan::DAYS - 1));
	CPPUNIT_ASSERT(!rules.accept(days(60), 10 * Timespan::DAYS + 1));
}

}
