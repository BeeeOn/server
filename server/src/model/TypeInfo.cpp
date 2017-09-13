#include <cmath>
#include <string>

#include <Poco/Exception.h>
#include <Poco/NumberFormatter.h>

#include "model/TypeInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TypeInfo::TypeInfo()
{
}

TypeInfo::TypeInfo(const TypeInfo::ID &id):
	Entity(id)
{
}

void TypeInfo::setName(const string &name)
{
	m_name = name;
}

const string &TypeInfo::name() const
{
	return m_name;
}

void TypeInfo::setUnit(const string &unit)
{
	m_unit = unit;
}

const string &TypeInfo::unit() const
{
	return m_unit;
}

void TypeInfo::setRange(const Range &range)
{
	m_range = range;
}

const TypeInfo::Range &TypeInfo::range() const
{
	return m_range;
}

std::string TypeInfo::asString(const double v) const
{
	return to_string(v);
}

TypeInfo::Range::Range():
	m_min(NAN),
	m_max(NAN),
	m_step(NAN)
{
}

TypeInfo::Range::Range(double min, double max, double step):
	m_min(min),
	m_max(max),
	m_step(step)
{
	if (m_min > m_max)
		throw InvalidArgumentException("min is greater then max");

	if (m_step <= 0)
		throw InvalidArgumentException("step must be positive");
}

bool TypeInfo::Range::hasMin() const
{
	return !std::isnan(m_min);
}

double TypeInfo::Range::min() const
{
	return m_min;
}

bool TypeInfo::Range::hasMax() const
{
	return !std::isnan(m_max);
}

double TypeInfo::Range::max() const
{
	return m_max;
}

bool TypeInfo::Range::hasStep() const
{
	return !std::isnan(m_step);
}

double TypeInfo::Range::step() const
{
	return m_step;
}

bool TypeInfo::Range::isValid() const
{
	return hasStep() || hasMin() || hasMax();
}
