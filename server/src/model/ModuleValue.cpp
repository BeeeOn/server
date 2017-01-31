#include <cmath>

#include "model/ModuleValue.h"

using namespace std;
using namespace BeeeOn;

ModuleValue::ModuleValue():
	m_module(),
	m_value(nan(""))
{
}

ModuleValue::ModuleValue(const ModuleInfoID &module, double value):
	m_module(module),
	m_value(value)
{
}

void ModuleValue::setModule(const ModuleInfoID module)
{
	m_module = module;
}

ModuleInfoID ModuleValue::module() const
{
	return m_module;
}

void ModuleValue::setValue(double value)
{
	m_value = value;
}

double ModuleValue::value() const
{
	return m_value;
}
