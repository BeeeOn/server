#ifndef BEEEON_GATEWAY_ID_H
#define BEEEON_GATEWAY_ID_H

#include <cstdint>
#include <Poco/Exception.h>

namespace BeeeOn {

/**
 * Gateway ID is a globally unique ID generated during manufactoring
 * of each device. The format of the ID is defined as 16 decadic
 * digits: VDDD DDDD DDDD DDDC. The V denotes version, C is the DAMM
 * checksum and D represents data digits which are generated randomly.
 */
class GatewayID {
public:
	enum {
		LENGTH = 16
	};

	GatewayID():
		m_value(0)
	{
	}

	GatewayID(uint64_t value):
		m_value(value)
	{
	}

	GatewayID(int version, uint64_t data);

	bool isNull() const
	{
		return m_value == 0;
	}

	int version() const
	{
		return std::to_string(m_value).at(0) - '0';
	}

	unsigned long data() const
	{
		return std::stol(std::to_string(m_value / 10).substr(1));
	}

	static GatewayID parse(const std::string &s);

	std::string toString() const
	{
		return std::to_string(m_value);
	}

	bool operator !=(const GatewayID &id) const
	{
		return m_value != id.m_value;
	}

	bool operator ==(const GatewayID &id) const
	{
		return m_value == id.m_value;
	}

	bool operator <(const GatewayID &id) const
	{
		return m_value < id.m_value;
	}

	bool operator >(const GatewayID &id) const
	{
		return m_value > id.m_value;
	}

	bool operator <=(const GatewayID &id) const
	{
		return m_value <= id.m_value;
	}

	bool operator >=(const GatewayID &id) const
	{
		return m_value >= id.m_value;
	}

	operator unsigned long long() const
	{
		return (unsigned long long) m_value;
	}

	operator uint64_t() const
	{
		return m_value;
	}

	/**
	 * Generate a random ID. Seed the random generator with the
	 * given seed. If the seed is 0 then the generator is possibly
	 * seeded in another (more secure) way.
	 */
	static GatewayID random(int version = 1, uint32_t seed = 0);

private:
	uint64_t m_value;
};

}

#endif
