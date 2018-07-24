#pragma once

#include "math/SimpleCalc.h"
#include "model/ModuleInfo.h"
#include "util/Loggable.h"

namespace BeeeOn {

/**
 * @brief UnknownEvaluator implements evaluation of formulas as
 * specified in devices.xml to convert from a raw value given
 * from a sensor into the right semantics (target type).
 */
class UnknownEvaluator : Loggable {
public:
	/**
	 * @brief For the given module, convert the raw value
	 * into the value as specified by ModuleInfo. The property
	 * ModuleInfo::fromUnknown() is required to be non-empty.
	 *
	 * The value raw must be a normal floating point value otherwise
	 * the method returns NAN.
	 *
	 * @throws Poco::IllegalStateException when fromUnknown() is empty
	 */
	double fromRaw(const ModuleInfo &info, double raw) const;

	/**
	 * @brief For the given module, convert the given value of
	 * the type as specified by the ModuleInfo into its raw
	 * form to pass it to the target controllable module.
	 * The property ModuleInfo::toUnknown() is required to be non-empty.
	 *
	 * The value raw must be a normal floating point value otherwise
	 * the method returns NAN.
	 *
	 * @throws Poco::IllegalStateException when toUnknown() is empty
	 */
	double toRaw(const ModuleInfo &info, double value) const;

private:
	SimpleCalc m_calc;
};

}
