#ifndef BEEEON_DEVICE_ID_H
#define BEEEON_DEVICE_ID_H

#include <cstdint>
#include <string>

namespace BeeeOn {

/**
 * Device ID. Called EUID in the adapter software.
 * An EUID contains a prefix (1 byte) that determines the device's domain.
 * It usually denotes a vendor. An EUID is globally unique.
 */
class DeviceID {
public:
	DeviceID();
	DeviceID(uint64_t value);
	DeviceID(uint8_t prefix, uint64_t ident);
	DeviceID(const DeviceID &copy);

	bool isNull() const
	{
		return m_value == 0;
	}

	operator unsigned long long() const
	{
		return (unsigned long long) m_value;
	}

	uint8_t prefix() const
	{
		return (uint8_t) (m_value >> 56);
	}

	uint64_t ident() const
	{
		return m_value & 0x00ffffffffffffffUL;
	}

	static DeviceID parse(const std::string &s);
	std::string toString() const;

	bool operator !=(const DeviceID &id) const
	{
		return m_value != id.m_value;
	}

	bool operator ==(const DeviceID &id) const
	{
		return m_value == id.m_value;
	}

	bool operator <(const DeviceID &id) const
	{
		return m_value < id.m_value;
	}

	bool operator >(const DeviceID &id) const
	{
		return m_value > id.m_value;
	}

	bool operator <=(const DeviceID &id) const
	{
		return m_value <= id.m_value;
	}

	bool operator >=(const DeviceID &id) const
	{
		return m_value >= id.m_value;
	}

	/**
	 * Generate a random ID. If the prefix is zero a new one is
	 * generated randomly.
	 */
	static DeviceID random(uint8_t prefix = 0);

private:
	uint64_t m_value;
};

}

#endif
