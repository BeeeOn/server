#include <cmath>

#include "model/ValueAt.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ValueAt::ValueAt():
	m_at(0),
	m_value(nan(""))
{
}

ValueAt::ValueAt(const unsigned long at, double value):
	m_at(at),
	m_value(value)
{
}

ValueAt::ValueAt(const Timestamp &at, double value):
	m_at(at.epochTime()),
	m_value(value)
{
}

void ValueAt::setAtRaw(const unsigned long at)
{
	m_at = at;
}

unsigned long ValueAt::atRaw() const
{
	return m_at;
}

void ValueAt::setAt(const Timestamp &at)
{
	setAtRaw(at.epochTime());
}

Timestamp ValueAt::at() const
{
	return Timestamp(m_at);
}

void ValueAt::setValue(double v)
{
	m_value = v;
}

double ValueAt::value() const
{
	return m_value;
}

bool ValueAt::isValid() const
{
	return !std::isnan(m_value);
}
