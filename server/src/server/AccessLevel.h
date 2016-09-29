#ifndef BEEEON_ACCESS_LEVEL_H
#define BEEEON_ACCESS_LEVEL_H

#include <string>
#include <climits>
#include <Poco/Exception.h>

namespace BeeeOn {

class AccessLevel {
public:
	AccessLevel();
	AccessLevel(unsigned int level);
	AccessLevel(const AccessLevel &level);

	enum {
		ANY    = 0,
		ADMIN  = 10,
		USER   = 20,
		GUEST  = 30,
		NONE   = 100
	};

	/**
	 * Parse level from string.
	 */
	static const AccessLevel &parse(const std::string &level);

	static const AccessLevel &any();
	static const AccessLevel &admin();
	static const AccessLevel &user();
	static const AccessLevel &guest();
	static const AccessLevel &none();

	/**
	 * Print parsable string representation of the level.
	 */
	const std::string toString() const;

	AccessLevel &operator =(const AccessLevel &level)
	{
		m_value = level.m_value;
		return *this;
	}

	bool operator ==(const AccessLevel &level) const
	{
		return m_value == level.m_value;
	}

	bool operator !=(const AccessLevel &level) const
	{
		return m_value != level.m_value;
	}

	bool operator <(const AccessLevel &level) const
	{
		return m_value < level.m_value;
	}

	bool operator >(const AccessLevel &level) const
	{
		return m_value > level.m_value;
	}

	bool operator <=(const AccessLevel &level) const
	{
		return m_value <= level.m_value;
	}

	bool operator >=(const AccessLevel &level) const
	{
		return m_value <= level.m_value;
	}

	operator unsigned int() const
	{
		return m_value;
	}

	operator int() const
	{
		if (m_value > INT_MAX)
			throw Poco::BadCastException("out of range of int");

		return (int) m_value;
	}

private:
	unsigned int m_value;
};

}

#endif
