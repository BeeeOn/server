#include <Poco/Exception.h>

#include "di/Injectable.h"
#include "policy/RuledSensorHistoryAccessPolicy.h"

BEEEON_OBJECT_BEGIN(BeeeOn, RuledSensorHistoryAccessPolicy)
BEEEON_OBJECT_CASTABLE(SensorHistoryAccessPolicy)
BEEEON_OBJECT_PROPERTY("innerPolicy", &RuledSensorHistoryAccessPolicy::setInnerPolicy)
BEEEON_OBJECT_PROPERTY("rules", &RuledSensorHistoryAccessPolicy::setRules)
BEEEON_OBJECT_END(BeeeOn, RuledSensorHistoryAccessPolicy)

using namespace Poco;
using namespace BeeeOn;

void RuledSensorHistoryAccessPolicy::setInnerPolicy(
		SensorHistoryAccessPolicy::Ptr policy)
{
	m_policy = policy;
}

void RuledSensorHistoryAccessPolicy::setRules(
		SensorHistoryRules::Ptr rules)
{
	m_rules = rules;
}

void RuledSensorHistoryAccessPolicy::assure(
		const Action action,
		const PolicyContext &context,
		const Device &device,
		const TimeInterval &range,
		const Timespan &interval)
{
	if (!m_rules->accept(range, interval)) {
		throw InvalidArgumentException(
			"given range & interval are disallowed by rules");
	}

	m_policy->assure(action, context, device, range, interval);
}
