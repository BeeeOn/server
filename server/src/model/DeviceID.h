#ifndef BEEEON_DEVICE_ID_H
#define BEEEON_DEVICE_ID_H

#include <cstdint>
#include <string>

namespace BeeeOn {

/**
 * Device ID. Called EUID in the adapter software.
 * An EUID contains a prefix (1 byte) that determines the device's domain.
 * It usually denotes a vendor. An EUID is globally unique.
 * The class supports both 32-bit and 64-bit EUIDs. The 32-bit EUID
 * is recognized by having the higher half of the uint64_t equal to zero.
 */
class DeviceID {
public:
	/**
	 * Construct an empty ID. The isNull() method would return true.
	 */
	DeviceID();

	/**
	 * Cosntruct 32-bit or 64-bit ID. Having the higher half of the
	 * value equal to zero enables the 32-bit mode. E.g.:
	 *
	 * - 0x00000000ac456571 is 32-bit
	 * - 0x5400000014240342 is 64-bit
	 */
	DeviceID(uint64_t value);

	/**
	 * Construct 64-bit ID only.
	 */
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
		return (uint8_t) (m_value >> (is32bit()? 24 : 56));
	}

	uint64_t ident() const
	{
		return m_value & (is32bit()?
				0x0000000000ffffffUL :
				0x00ffffffffffffffUL);
	}

	bool is32bit() const
	{
		return (m_value >> 32) == 0;
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
