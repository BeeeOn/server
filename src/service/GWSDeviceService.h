#ifndef BEEEON_GWS_DEVICE_SERVICE_H
#define BEEEON_GWS_DEVICE_SERVICE_H

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "model/Gateway.h"
#include "model/DevicePrefix.h"

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
	 *
	 * @throw InvalidArgumentException for invalid name + vendor combination.
	 *
	 * @return false if device update or insertion fails.
	 */
	virtual bool registerDevice(Device &device,
		const std::string &name,
		const std::string &vendor,
		const Gateway &gateway) = 0;

	/**
	 * Retrieve vector of all active devices owned by the given gateway,
	 * whose device id is prefixed with the given prefix.
	 */
	virtual void fetchActiveWithPrefix(std::vector<Device> &devices,
		const Gateway &gateway,
		const DevicePrefix &prefix) = 0;
};

}

#endif