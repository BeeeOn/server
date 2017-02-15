#ifndef BEEEON_VALUE_GENERATOR_H
#define BEEEON_VALUE_GENERATOR_H

namespace Poco {

class DateTime;
class Timestamp;
class Timespan;

}

namespace BeeeOn {

class ValueGenerator {
public:
	ValueGenerator();
	virtual ~ValueGenerator();

	virtual bool hasNext() const;
	virtual double next() = 0;
};

class LimitedGenerator : public ValueGenerator {
public:
	LimitedGenerator(ValueGenerator &impl, unsigned int count);

	virtual bool hasNext() const;
	virtual double next();

private:
	ValueGenerator &m_impl;
	unsigned int m_count;
};

class RangeGenerator : public ValueGenerator {
public:
	RangeGenerator(ValueGenerator &impl, double min, double max);

	virtual double next();

private:
	ValueGenerator &m_impl;
	double m_min;
	double m_max;
};

class TimestampedGenerator : public ValueGenerator {
public:
	TimestampedGenerator(ValueGenerator &impl,
			const Poco::DateTime &start,
			const Poco::Timespan step);
	TimestampedGenerator(ValueGenerator &impl,
			const Poco::DateTime &start,
			unsigned int stepSecs);

	virtual bool hasNext() const;
	virtual double next();
	virtual Poco::Timestamp at() const;

private:
	ValueGenerator &m_impl;
	Poco::Timestamp m_at;
	Poco::Timespan m_step;
};

class TimeLimitedGenerator : public ValueGenerator {
public:
	TimeLimitedGenerator(TimestampedGenerator &impl,
			const Poco::DateTime &before);
	TimeLimitedGenerator(TimestampedGenerator &impl,
			const Poco::Timestamp &before);

	virtual bool hasNext() const;
	virtual double next();
	virtual Poco::Timestamp at() const;

private:
	TimestampedGenerator m_impl;
	Poco::Timestamp m_before;
};

class ConstGenerator : public ValueGenerator {
public:
	ConstGenerator(double value);

	double next();

private:
	double m_value;
};

class SinGenerator : public ValueGenerator {
public:
	SinGenerator(double step = 0.01);

	double next();

private:
	double m_value;
	double m_step;
};

class RandomGenerator : public ValueGenerator {
public:
	RandomGenerator(unsigned int seed = 0);

	double next();

private:
	unsigned int m_seed;
};

}

#endif
