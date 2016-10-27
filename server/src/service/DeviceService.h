#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include <vector>

#include "di/InjectorTarget.h"
#include "service/Relation.h"
#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class DeviceDao;
class GatewayRPC;

/**
 * Service for devices management.
 */
class DeviceService : public AbstractInjectorTarget {
public:
	DeviceService();

	void setDeviceDao(DeviceDao *dao);
	void setGatewayRPC(GatewayRPC *rpc);

	bool fetch(Relation<Device, Gateway> &input);
	void fetchActiveBy(Relation<std::vector<Device>, Gateway> &input);
	void fetchInactiveBy(Relation<std::vector<Device>, Gateway> &input);

	bool activate(Relation<Device, Gateway> &input);
	bool unregister(Relation<Device, Gateway> &input);

protected:
	bool tryActivateAndUpdate(Device &device, const Gateway &gateway);

private:
	DeviceDao *m_dao;
	GatewayRPC *m_gatewayRPC;
};

}

#endif
