#ifndef BEEEON_DEVICE_H
#define BEEEON_DEVICE_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"

namespace BeeeOn {

/**
 * Representation of the Device application entity.
 */
class Device : public Entity {
public:
	typedef Poco::SharedPtr<Device> Ptr;
	typedef long ID;

	Device(const std::string &data):
		Entity(data)
	{
	}

	Device(const Device &device):
		Entity(device)
	{
	}
};

typedef Device::Collection DeviceCollection;
typedef Device::ID DeviceID;

}

#endif
