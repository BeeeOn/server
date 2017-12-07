#ifndef BEEEON_MODULE_VALUE_H
#define BEEEON_MODULE_VALUE_H

#include "model/ModuleInfo.h"

namespace BeeeOn {

/**
 * This class represents a value of a module together with
 * the ID of module it is part of. It is not intended to be
 * used for large vectors or arrays.
 */
class ModuleValue {
public:
	ModuleValue();
	ModuleValue(const ModuleInfoID &module, double value);

	void setModule(const ModuleInfoID module);
	ModuleInfoID module() const;

	void setValue(double value);
	double value() const;

	bool isValid() const;

private:
	ModuleInfoID m_module;
	double m_value;
};

}

#endif
