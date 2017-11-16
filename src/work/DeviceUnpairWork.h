#ifndef BEEEON_DEVICE_UNPAIR_WORK_H
#define BEEEON_DEVICE_UNPAIR_WORK_H

#include "model/Device.h"
#include "model/Gateway.h"
#include "work/AbstractGatewayWork.h"

namespace BeeeOn {

/**
 * Representation of a work that unpairs a device from its gateway.
 * The unpairing process usually requires some asynchronous communication
 * with other parts of the system.
 */
class DeviceUnpairWork : public AbstractGatewayWork {
public:
	DeviceUnpairWork();
	DeviceUnpairWork(const WorkContent &content);
	DeviceUnpairWork(const Poco::JSON::Object::Ptr json);

	/**
	 * ID of gateway where a device is to be unpaired from.
	 */
	void setGatewayID(const GatewayID &id);
	GatewayID gatewayID() const;

	/**
	 * ID of device to be unpaired.
	 */
	void setDeviceID(const DeviceID &id);
	DeviceID deviceID() const;
};

}

#endif
