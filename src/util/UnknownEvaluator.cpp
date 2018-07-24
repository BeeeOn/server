#include <cmath>
#include <string>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/String.h>

#include "util/UnknownEvaluator.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

static const string PAT_RAW = "${raw}";
static const string PAT_VALUE = "${value}";

double UnknownEvaluator::fromRaw(const ModuleInfo &info, double raw) const
{
	auto formula = info.fromUnknown();
	if (formula.empty()) {
		throw IllegalStateException(
			"no conversion from unknown raw value for "
			+ info.toString());
	}

	if (raw != 0.0 && !std::isnormal(raw))
		return NAN;

	replaceInPlace(formula, PAT_RAW, to_string(raw));

	if (logger().debug()) {
		logger().debug(
			"evaluating from raw '" + formula + "'",
			__FILE__, __LINE__);
	}

	return m_calc.evaluate(formula);
}

double UnknownEvaluator::toRaw(const ModuleInfo &info, double value) const
{
	auto formula = info.toUnknown();
	if (formula.empty()) {
		throw IllegalStateException(
			"no conversion to unknown raw value for "
			+ info.toString());
	}

	if (value != 0.0 && !std::isnormal(value))
		return NAN;

	replaceInPlace(formula, PAT_VALUE, to_string(value));

	if (logger().debug()) {
		logger().debug(
			"evaluating to raw '" + formula + "'",
			__FILE__, __LINE__);
	}

	return m_calc.evaluate(formula);
}
