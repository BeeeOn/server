#pragma once

#include <map>

#include <Poco/SharedPtr.h>
#include <Poco/Timespan.h>

#include "util/Loggable.h"
#include "util/TimeInterval.h"

namespace BeeeOn {

/**
 * @brief SensorHistoryRules implements check of parameters used when
 * asking for sensor history data. In this way, it is possible to
 * restrict requests to a supported range of request or to forbid
 * requests which are too computational intensive.
 */
class SensorHistoryRules : Loggable {
public:
	typedef Poco::SharedPtr<SensorHistoryRules> Ptr;

	struct Span {
		const Poco::Timespan min;
		const Poco::Timespan max;

		Span(const Poco::Timespan &min, const Poco::Timespan &max);

		bool operator<(const Span &) const;
		std::string toString() const;
	};

	SensorHistoryRules();
	virtual ~SensorHistoryRules();

	bool accept(
		const TimeInterval &range,
		const Poco::Timespan &interval) const;

	/**
	 * Set rules by parsing the map keys and values.
	 * It recognizes rule '*' as the missing rule. If the '*' rule is
	 * not present, the missing rule is cleared.
	 *
	 * @see SensorHistoryRules::parseSpan()
	 */
	void parseAndSetRules(const std::map<std::string, std::string> &rules);

	void setRules(const std::map<Span, Span> &rules);
	void setMissing(Poco::SharedPtr<Span> interval);
	void setAcceptZeroInterval(bool accept);

	/**
	 * @brief Parse the given span specification.
	 *
	 * It accepts inputs like: <code>1 s</code>, <code>1 m</code>,
	 * <code>5 s..1 m</code>, <code>5 s..1 m+</code>, <code>5 m..max</code>.
	 *
	 * The '+' suffix is used to determine inclusive interval. Consider
	 * that <code>1 s..1 m</code> is equivalent to <code>1000000 us..60000000 us</code>.
	 * But <code>1 s..1 m+<code> is equivalent to <code>1000000 us..60000001 us</code>.
	 *
	 * The syntax rules:
	 *
	 * - <code>NUMBER UNIT</code>
	 * - <code>max</code>
	 * - <code>NUMBER UNIT PLUS?..NUMBER UNIT PLUS?</code>
	 * - <code>NUMBER UNIT PLUS?..max</code>
	 *
	 * The NUMBER is treated as <code>[0-9]+</code>.
	 *
	 * The UNIT is treated as <code>{us|ms|s|m|h|d|}</code> (empty is possble).
	 *
	 * The PLUS is treated as <code>\+</code>
	 *
	 * @throws Poco::SyntaxException
	 * @throws Poco::InvalidArgumentException when left > right.
	 *
	 * @see TimespanParser::parse()
	 */
	static Span parseSpan(const std::string &input);

protected:
	static std::string asString(const Poco::Timespan &time);
	static Poco::Timespan parseTime(
		const std::string &input,
		bool max = false);

private:
	std::map<Span, Span> m_rules;
	Poco::SharedPtr<Span> m_missing;
	bool m_acceptZeroInterval;
};

}
