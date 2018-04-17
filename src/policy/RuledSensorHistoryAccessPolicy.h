#pragma once

#include "policy/SensorHistoryAccessPolicy.h"
#include "policy/SensorHistoryRules.h"

namespace BeeeOn {

/**
 * @brief Apply SensorHistoryRules to sensor history requests.
 */
class RuledSensorHistoryAccessPolicy : public SensorHistoryAccessPolicy {
public:
	typedef Poco::SharedPtr<RuledSensorHistoryAccessPolicy> Ptr;

	void setInnerPolicy(SensorHistoryAccessPolicy::Ptr policy);
	void setRules(SensorHistoryRules::Ptr rules);

	/**
	 * Assure that the given request meets the rules and
	 * than delegate the check to the inner policy.
	 *
	 * @throws Poco::InvalidArgumentException when rules are violated
	 */
	void assure(
		const Action action,
		const PolicyContext &context,
		const Device &device,
		const TimeInterval &range,
		const Poco::Timespan &interval) override;

private:
	SensorHistoryAccessPolicy::Ptr m_policy;
	SensorHistoryRules::Ptr m_rules;
};

}
