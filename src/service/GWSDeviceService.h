#ifndef BEEEON_GWS_DEVICE_SERVICE_H
#define BEEEON_GWS_DEVICE_SERVICE_H

#include <list>
#include <vector>
#include <Poco/SharedPtr.h>

#include "model/DeviceWithData.h"
#include "model/Gateway.h"
#include "model/DeviceDescription.h"
#include "model/DevicePrefix.h"
#include "model/ModuleType.h"

namespace BeeeOn {

class GWSDeviceService {
public:
	typedef Poco::SharedPtr<GWSDeviceService> Ptr;

	virtual ~GWSDeviceService();

	/**
	 * Register the given device to be owned by the given gateway.
	 *
	 * It is intended to use after discovering new devices on the gateway.
	 * Device type is recognized by the name + vendor combination. In case of
	 * repeated registration, device is just updated with new last seen time.
	 * The reported module types of the device are verified.
	 *
	 * @throw NotFoundException for invalid name + vendor combination.
	 * @throw InvalidArgumentException for invalid count of modules.
	 * @throw MultiException for invalid types of modules.
	 *
	 * @return false if device update or insertion fails.
	 */
	virtual bool registerDevice(Device &device,
		const DeviceDescription &description,
		const Gateway &gateway) = 0;

	/**
	 * Register the given group of devices to be owned by the given gateway.
	 *
	 * It is intended to register a new group device consisting of
	 * subdevices which are logically related somehow. Device type is
	 * recognized by the name + vendor combination. In case of repeated
	 * registrations, such device is just updated with new last seen time.
	 * The reported module types of the devices are verified.
	 *
	 * @throw NotFoundException for invalid name + vendor combination.
	 * @throw InvalidArgumentException for invalid count of modules.
	 * @throw MultiException for invalid types of modules.
	 * @throw IllegalStateException for unsuccessful registration.
	 */
	virtual void registerDeviceGroup(
		const std::vector<DeviceDescription> &descriptions,
		const Gateway &gateway) = 0;

	/**
	 * Retrieve vector of all active devices owned by the given gateway,
	 * whose device id is prefixed with the given prefix.
	 */
	virtual void fetchActiveWithPrefix(std::vector<DeviceWithData> &devices,
		const Gateway &gateway,
		const DevicePrefix &prefix) = 0;
};

}

#endif
