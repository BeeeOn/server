#pragma once

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class DeviceEvent {
public:
	DeviceEvent();

	void setGatewayID(const GatewayID &id);
	const GatewayID &gatewayID() const;

	void setDeviceID(const DeviceID &id);
	const DeviceID &deviceID() const;

private:
	GatewayID m_gatewayID;
	DeviceID m_deviceID;
};

class DeviceListener {
public:
	typedef Poco::SharedPtr<DeviceListener> Ptr;

	virtual ~DeviceListener();

	/**
	 * Fired when a new device is discovered and registered in the system.
	 */
	virtual void onNewDevice(const DeviceEvent &e);

	/**
	 * Fired when a new device is discovered but never registred due to
	 * some failure (e.g. incompatibility).
	 */
	virtual void onRefusedNewDevice(const DeviceEvent &e);
};

}
