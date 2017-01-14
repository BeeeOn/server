#ifndef BEEEON_SIMPLE_ID_H
#define BEEEON_SIMPLE_ID_H

#include <string>
#include <climits>
#include <Poco/Exception.h>
#include <Poco/NumberParser.h>

namespace BeeeOn {

class SimpleID {
public:
	SimpleID():
		m_value(0)
	{
	}

	SimpleID(long value):
		m_value(value)
	{
	}

	SimpleID(const SimpleID &copy):
		m_value(copy.m_value)
	{
	}

	bool isNull() const
	{
		return m_value == 0;
	}

	operator long() const
	{
		return m_value;
	}

	operator int() const
	{
		if (INT_MIN > m_value || m_value > INT_MAX)
			throw Poco::BadCastException("out of range of int");

		return (int) m_value;
	}

	operator unsigned int() const
	{
		if (m_value < 0 || m_value > INT_MAX)
			throw Poco::BadCastException("out of range of unsigned int");

		return (unsigned int) m_value;
	}

	static SimpleID parse(const std::string &s)
	{
		if (s[0] == '0' && s[1] == 'x')
			return SimpleID(Poco::NumberParser::parseHex(s));

		return SimpleID(Poco::NumberParser::parse(s));
	}

	std::string toString() const
	{
		return std::to_string(m_value);
	}

	SimpleID &operator ++()
	{
		++m_value;
		return *this;
	}

	SimpleID &operator ++(int)
	{
		m_value++;
		return *this;
	}

	bool operator !=(const SimpleID &id) const
	{
		return m_value != id.m_value;
	}

	bool operator ==(const SimpleID &id) const
	{
		return m_value == id.m_value;
	}

	bool operator <(const SimpleID &id) const
	{
		return m_value < id.m_value;
	}

	bool operator >(const SimpleID &id) const
	{
		return m_value > id.m_value;
	}

	bool operator <=(const SimpleID &id) const
	{
		return m_value <= id.m_value;
	}

	bool operator >=(const SimpleID &id) const
	{
		return m_value >= id.m_value;
	}

private:
	long m_value = 0;
};

}

#endif
