#pragma once

#include <string>

#include <Poco/SharedPtr.h>

#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class DeviceEvent {
public:
	DeviceEvent();
	DeviceEvent(const GatewayID &gatewayID, const DeviceID &deviceID);

	void setGatewayID(const GatewayID &id);
	const GatewayID &gatewayID() const;

	void setDeviceID(const DeviceID &id);
	const DeviceID &deviceID() const;

	/**
	 * @brief Set name of the related device.
	 */
	void setName(const std::string &name);

	/**
	 * @returns name of the related device (might be intentionally
	 * empty for certain types of events).
	 */
	const std::string &name() const;

private:
	GatewayID m_gatewayID;
	DeviceID m_deviceID;
	std::string m_name;
};

class DeviceListener {
public:
	typedef Poco::SharedPtr<DeviceListener> Ptr;

	virtual ~DeviceListener();

	/**
	 * Fired when a new device is discovered and registered in the system.
	 * The event should contain the name of the related device if there is some.
	 */
	virtual void onNewDevice(const DeviceEvent &e);

	/**
	 * Fired when a new device is discovered but never registred due to
	 * some failure (e.g. incompatibility).
	 */
	virtual void onRefusedNewDevice(const DeviceEvent &e);
};

}
