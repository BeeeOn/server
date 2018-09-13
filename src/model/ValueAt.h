#pragma once

#include <Poco/Timestamp.h>

namespace BeeeOn {

class ValueAt {
public:
	ValueAt();
	ValueAt(const unsigned long at, double value);
	ValueAt(const Poco::Timestamp &at, double value);

	void setAtRaw(const unsigned long at);
	unsigned long atRaw() const;

	void setAt(const Poco::Timestamp &at);
	Poco::Timestamp at() const;

	void setValue(double v);
	double value() const;

	bool isValid() const;

private:
	unsigned long m_at;
	double m_value;
};

}
