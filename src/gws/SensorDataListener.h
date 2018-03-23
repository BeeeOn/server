#ifndef BEEEON_SENSOR_DATA_LISTENER_H
#define BEEEON_SENSOR_DATA_LISTENER_H

#include <vector>

#include <Poco/SharedPtr.h>
#include <Poco/Timestamp.h>

#include "model/Device.h"
#include "model/Gateway.h"
#include "model/ModuleValue.h"

namespace BeeeOn {

/**
 * @brief SensorDataEvent holds the received data and metadata
 * describing details. Each such event is related to a single
 * remote gateway and to a single remote device associated with
 * that gateway. The timestamp when the data has been measured
 * is included.
 */
class SensorDataEvent {
public:
	SensorDataEvent();

	void setGatewayID(const GatewayID &id);
	const GatewayID &gatewayID() const;

	void setDeviceID(const DeviceID &id);
	const DeviceID &deviceID() const;

	void setStamp(const Poco::Timestamp &stamp);
	const Poco::Timestamp &stamp() const;

	void setData(const std::vector<ModuleValue> &data);
	const std::vector<ModuleValue> &data() const;

private:
	GatewayID m_gatewayID;
	DeviceID m_deviceID;
	Poco::Timestamp m_stamp;
	std::vector<ModuleValue> m_data;
};

/**
 * @brief SensorDataListener provides capability to received
 * events when sensor data comes from a remote gateway.
 */
class SensorDataListener {
public:
	typedef Poco::SharedPtr<SensorDataListener> Ptr;

	virtual ~SensorDataListener();

	/**
	 * @brief Notify that we have just received data.
	 */
	virtual void onReceived(const SensorDataEvent &e);
};

}

#endif
