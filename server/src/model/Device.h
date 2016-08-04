#ifndef BEEEON_DEVICE_H
#define BEEEON_DEVICE_H

#include <Poco/SharedPtr.h>
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of the Device application entity.
 */
class Device {
public:
	typedef Poco::SharedPtr<Device> Ptr;
	typedef BeeeOn::Collection<Device> Collection;
	typedef long ID;

	Device()
	{
	}

	Device(const Device &copy):
		m_name(copy.m_name)
	{
	}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const
	{
		return m_name;
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void toWeb(Serializer &s) const
	{
		s.push("name", m_name);
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void fromWeb(Serializer &s)
	{
		s.get("name", m_name);
	}

private:
	std::string m_name;
};

typedef Device::Collection DeviceCollection;
typedef Device::ID DeviceID;

}

#endif
