#ifndef BEEEON_GATEWAY_ID_H
#define BEEEON_GATEWAY_ID_H

#include <cstdint>
#include <Poco/Exception.h>

namespace BeeeOn {

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

	static GatewayID random(int version = 1);

private:
	uint64_t m_value;
};

}

#endif
