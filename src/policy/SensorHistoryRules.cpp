#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/String.h>

#include "di/Injectable.h"
#include "policy/SensorHistoryRules.h"
#include "util/TimespanParser.h"

BEEEON_OBJECT_BEGIN(BeeeOn, SensorHistoryRules)
BEEEON_OBJECT_PROPERTY("rules", &SensorHistoryRules::parseAndSetRules)
BEEEON_OBJECT_PROPERTY("acceptZeroInterval", &SensorHistoryRules::setAcceptZeroInterval)
BEEEON_OBJECT_END(BeeeOn, SensorHistoryRules)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SensorHistoryRules::Span::Span(
		const Timespan &_min,
		const Timespan &_max):
	min(_min),
	max(_max)
{
	if (min > max)
		throw InvalidArgumentException("min > max for " + toString());
}

bool SensorHistoryRules::Span::operator <(const Span &other) const
{
	return (min < other.min) && (max <= other.min);
}

string SensorHistoryRules::Span::toString() const
{
	if (min == max)
		return SensorHistoryRules::asString(min);

	return SensorHistoryRules::asString(min)
		+ ".."
		+ SensorHistoryRules::asString(max);
}

string SensorHistoryRules::asString(const Timespan &time)
{
	if (time == 0)
		return "0";

	if (time.microseconds() > 0)
		return to_string(time.totalMicroseconds()) + " us";
	if (time.milliseconds() > 0)
		return to_string(time.totalMilliseconds()) + " ms";
	if (time.seconds() > 0)
		return to_string(time.totalSeconds()) + " s";
	if (time.minutes() > 0)
		return to_string(time.totalMinutes()) + " m";
	if (time.hours() > 0)
		return to_string(time.totalHours()) + " h";

	return to_string(time.days()) + " d";
}

SensorHistoryRules::SensorHistoryRules():
	m_acceptZeroInterval(false)
{
}

SensorHistoryRules::~SensorHistoryRules()
{
}

void SensorHistoryRules::parseAndSetRules(const std::map<std::string, std::string> &rules)
{
	map<Span, Span> spans;
	SharedPtr<Span> missing;

	for (const auto &pair : rules) {
		const string &key = trim(pair.first);
		const string &value = trim(pair.second);

		if (key == "*") {
			if (!missing.isNull())
				throw ExistsException("duplicate '*' rule");

			missing = new Span(parseSpan(value));
		}
		else {
			const Span &span = parseSpan(key);
			const Span &interval = parseSpan(value);

			logger().information(
				"registering rule "
				+ span.toString()
				+ " -> "
				+ interval.toString(),
				__FILE__, __LINE__);

			const auto result = spans.emplace(span, interval);

			if (!result.second) {
				const auto conflict = result.first->first;

				throw ExistsException(
					"span "
					+ span.toString()
					+ " is overlapping with "
					+ conflict.toString());
			}
		}
	}

	setRules(spans);
	setMissing(missing);
}

void SensorHistoryRules::setRules(const std::map<Span, Span> &rules)
{
	m_rules = rules;
}

void SensorHistoryRules::setMissing(SharedPtr<Span> interval)
{
	m_missing = interval;
}

void SensorHistoryRules::setAcceptZeroInterval(bool accept)
{
	m_acceptZeroInterval = accept;
}

bool SensorHistoryRules::accept(
		const TimeInterval &range,
		const Timespan &interval) const
{
	const Timespan &span = range.end() - range.start();
	const Span &needle = {span, span};

	if (interval == 0 && !m_acceptZeroInterval) {
		if (logger().debug()) {
			logger().debug(
				"zero interval not allowed for "
				+ needle.toString(),
				__FILE__, __LINE__);
		}

		return false;
	}

	const auto it = m_rules.find(needle);
	if (it == m_rules.end() && m_missing.isNull()) {
		if (logger().error()) {
			logger().error(
				"no such rule for "
				+ needle.toString(),
				__FILE__, __LINE__);
		}

		return false;
	}
	else if (it == m_rules.end()) {
		if (logger().debug()) {
			logger().debug(
				"applying the rule 'missing' for "
				+ needle.toString(),
				__FILE__, __LINE__);
		}
	}
	else {
		if (logger().debug()) {
			logger().debug(
				"range "
				+ needle.toString()
				+ " matched with rule "
				+ it->first.toString(),
				__FILE__, __LINE__);
		}
	}

	const auto &rule = it == m_rules.end() ? *m_missing : it->second;
	const bool result = interval >= rule.min && interval <= rule.max;

	if (result) {
		if (logger().error()) {
			logger().error(
				"interval "
				+ asString(interval)
				+ " is not allowed for "
				+ rule.toString(),
				__FILE__, __LINE__);
		}
	}
	else {
		if (logger().debug()) {
			logger().debug(
				"interval "
				+ asString(interval)
				+ " allowed for "
				+ rule.toString(),
				__FILE__, __LINE__);
		}
	}

	return result;
}

Timespan SensorHistoryRules::parseTime(const string &input, bool max)
{
	static const Timespan TDIFF_MAX = Timestamp::TIMEVAL_MAX;

	const auto part = trim(input);
	if (max && part == "max")
		return TDIFF_MAX;

	const bool hasPlus = part.back() == '+';
	const string &noPlus = hasPlus ?
		trim(string(part.begin(), part.end() - 1)) : part;

	const Timespan &time = TimespanParser::parse(noPlus);
	return hasPlus ? time + 1 : time;
}

SensorHistoryRules::Span SensorHistoryRules::parseSpan(const string &input)
{
	const auto sep = input.find("..");
	if (sep == string::npos) {
		if (input.find("+") != string::npos)
			throw SyntaxException("'+' is not allowed for " + input);

		const auto span = parseTime(input, true);
		return {span, span};
	}

	const auto left = parseTime(input.substr(0, sep));
	const auto right = parseTime({input.begin() + sep + 2, input.end()}, true);

	return {left, right};
}
