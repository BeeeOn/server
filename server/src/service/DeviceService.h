#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include <list>
#include <vector>

#include "di/InjectorTarget.h"
#include "service/Relation.h"
#include "model/Device.h"
#include "model/Gateway.h"

namespace BeeeOn {

class DeviceDao;
class GatewayRPC;
class DeviceAccessPolicy;

/**
 * Service for devices management.
 */
class DeviceService : public AbstractInjectorTarget {
public:
	DeviceService();

	void setDeviceDao(DeviceDao *dao);
	void setGatewayRPC(GatewayRPC *rpc);
	void setAccessPolicy(DeviceAccessPolicy *policy);

	bool fetch(Relation<Device, Gateway> &input);
	void fetchMany(Relation<std::list<Device>, Gateway> &input);
	void fetchActiveBy(Relation<std::vector<Device>, Gateway> &input);
	void fetchInactiveBy(Relation<std::vector<Device>, Gateway> &input);

	bool activate(Relation<Device, Gateway> &input);
	bool unregister(Relation<Device, Gateway> &input);
	bool update(RelationWithData<Device, Gateway> &input);
	bool updateAndActivate(RelationWithData<Device, Gateway> &input);

protected:
	bool prepareUpdate(RelationWithData<Device, Gateway> &input);
	bool tryActivateAndUpdate(Device &device,
			const Gateway &gateway, bool forceUpdate = false);

private:
	DeviceDao *m_dao;
	GatewayRPC *m_gatewayRPC;
	DeviceAccessPolicy *m_policy;
};

}

#endif
