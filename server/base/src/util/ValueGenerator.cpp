#define _POSIX_C_SOURCE

#include <cmath>
#include <cstdlib>

#include <Poco/Exception.h>
#include <Poco/DateTime.h>
#include <Poco/Timespan.h>
#include <Poco/Timestamp.h>

#include "util/ValueGenerator.h"

using namespace Poco;
using namespace BeeeOn;

ValueGenerator::ValueGenerator()
{
}

ValueGenerator::~ValueGenerator()
{
}

bool ValueGenerator::hasNext() const
{
	return true;
}

LimitedGenerator::LimitedGenerator(ValueGenerator &impl, unsigned int count):
	m_impl(impl),
	m_count(count)
{
}

bool LimitedGenerator::hasNext() const
{
	return m_count > 0;
}

double LimitedGenerator::next()
{
	const double r = m_impl.next();
	m_count -= 1;
	return r;
}

RangeGenerator::RangeGenerator(ValueGenerator &impl, double min, double max):
	m_impl(impl),
	m_min(min),
	m_max(max)
{
	if (m_min >= m_max)
		throw InvalidArgumentException("min must be less then max");
}

double RangeGenerator::next()
{
	const double r = m_impl.next();
	return (r * std::fabs(m_max - m_min)) + m_min;
}

TimestampedGenerator::TimestampedGenerator(
		ValueGenerator &impl,
		const DateTime &start,
		const Timespan step):
	m_impl(impl),
	m_at(start.timestamp()),
	m_step(step)
{
}

TimestampedGenerator::TimestampedGenerator(
		ValueGenerator &impl,
		const DateTime &start,
		unsigned int stepSecs):
	m_impl(impl),
	m_at(start.timestamp()),
	m_step(stepSecs, 0)
{
}

bool TimestampedGenerator::hasNext() const
{
	return m_impl.hasNext();
}

double TimestampedGenerator::next()
{
	const double r = m_impl.next();
	m_at += m_step;

	return r;
}

Timestamp TimestampedGenerator::at() const
{
	return m_at;
}

TimeLimitedGenerator::TimeLimitedGenerator(
		TimestampedGenerator &impl,
		const DateTime &before):
	m_impl(impl),
	m_before(before.timestamp())
{
}

TimeLimitedGenerator::TimeLimitedGenerator(
		TimestampedGenerator &impl,
		const Timestamp &before):
	m_impl(impl),
	m_before(before)
{
}

bool TimeLimitedGenerator::hasNext() const
{
	if (m_impl.at() < m_before)
		return m_impl.hasNext();

	return false;
}

double TimeLimitedGenerator::next()
{
	return m_impl.next();
}

Timestamp TimeLimitedGenerator::at() const
{
	return m_impl.at();
}

ConstGenerator::ConstGenerator(double value):
	m_value(value)
{
}

double ConstGenerator::next()
{
	return m_value;
}

SinGenerator::SinGenerator(double step):
	m_value(0),
	m_step(step)
{
	if (m_step <= 0)
		throw InvalidArgumentException("step must be greater then 0");
}

double SinGenerator::next()
{
	const double r = std::sin(m_value);
	m_value += m_step;
	return r;
}

RandomGenerator::RandomGenerator(unsigned int seed):
	m_seed(seed)
{
}

double RandomGenerator::next()
{
	const double r = rand_r(&m_seed);
	return r / RAND_MAX;
}
