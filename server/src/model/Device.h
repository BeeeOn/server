#ifndef BEEEON_DEVICE_H
#define BEEEON_DEVICE_H

#include <Poco/SharedPtr.h>
#include "model/DeviceID.h"
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of the Device application entity.
 */
class Device {
public:
	typedef Poco::SharedPtr<Device> Ptr;
	typedef BeeeOn::Collection<Device> Collection;
	typedef DeviceID ID;

	Device()
	{
	}

	Device(const ID &id):
		m_id(id)
	{
	}

	Device(const Device &copy):
		m_id(copy.m_id),
		m_name(copy.m_name)
	{
	}

	Device(const ID &id, const Device &copy):
		m_id(id),
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

	const ID &id() const
	{
		return m_id;
	}

private:
	ID m_id;
	std::string m_name;
};

typedef Device::Collection DeviceCollection;
typedef Device::ID DeviceID;

}

#endif
