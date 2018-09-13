#pragma once

#include <Poco/SharedPtr.h>

#include "rest/JSONRestHandler.h"

namespace BeeeOn {

class DeviceService;
class GatewayService;

namespace RestUI {

class DeviceRestHandler : public JSONRestHandler {
public:
	DeviceRestHandler();

	void setDeviceService(Poco::SharedPtr<DeviceService> service);
	void setGatewayService(Poco::SharedPtr<GatewayService> service);

	void list(RestFlow &flow);
	void detail(RestFlow &flow);
	void update(RestFlow &flow);
	void discover(RestFlow &flow);
	void discoverStatus(RestFlow &flow);
	void unpair(RestFlow &flow);

private:
	Poco::SharedPtr<DeviceService> m_deviceService;
	Poco::SharedPtr<GatewayService> m_gatewayService;
};

}
}
