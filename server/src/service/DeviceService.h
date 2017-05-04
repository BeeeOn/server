#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include <list>
#include <vector>

#include "dao/Transactional.h"
#include "service/Single.h"
#include "service/Relation.h"
#include "model/Device.h"
#include "model/DeviceProperty.h"
#include "model/Gateway.h"
#include "work/Work.h"

namespace BeeeOn {

class DeviceDao;
class DevicePropertyDao;
class GatewayRPC;
class WorkScheduler;
class DeviceAccessPolicy;

/**
 * Service for devices management.
 */
class DeviceService : public Transactional {
public:
	DeviceService();

	void setDeviceDao(DeviceDao *dao);
	void setDevicePropertyDao(DevicePropertyDao *dao);
	void setGatewayRPC(GatewayRPC *rpc);
	void setWorkScheduler(WorkScheduler *scheduler);
	void setAccessPolicy(DeviceAccessPolicy *policy);

	bool fetch(Relation<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchMany(Single<std::list<Device>> &input)
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchMany(Relation<std::list<Device>, Gateway> &input)
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchActiveBy(Relation<std::vector<Device>, Gateway> &input)
	{
		BEEEON_TRANSACTION(doFetchActiveBy(input));
	}

	void fetchInactiveBy(Relation<std::vector<Device>, Gateway> &input)
	{
		BEEEON_TRANSACTION(doFetchInactiveBy(input));
	}

	bool activate(Relation<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doActivate(input));
	}

	Work::Ptr unregister(Relation<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(Work::Ptr, doUnregister(input));
	}

	bool update(RelationWithData<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdate(input));
	}

	bool updateAndActivate(RelationWithData<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdateAndActivate(input));
	}

	bool createProperty(RelationWithData<DeviceProperty, Device> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doCreateProperty(input));
	}

	bool updateProperty(RelationWithData<DeviceProperty, Device> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdateProperty(input));
	}

	bool removeProperty(Relation<const DeviceProperty, Device> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRemoveProperty(input));
	}

	bool findProperty(Relation<DeviceProperty, Device> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFindProperty(input));
	}

	void listProperties(Relation<std::list<DeviceProperty>, Device> &input)
	{
		return BEEEON_TRANSACTION(doListProperties(input));
	}

protected:
	bool doFetch(Relation<Device, Gateway> &input);
	void doFetchMany(Single<std::list<Device>> &input);
	void doFetchMany(Relation<std::list<Device>, Gateway> &input);
	void doFetchActiveBy(Relation<std::vector<Device>, Gateway> &input);
	void doFetchInactiveBy(Relation<std::vector<Device>, Gateway> &input);

	bool doActivate(Relation<Device, Gateway> &input);
	Work::Ptr doUnregister(Relation<Device, Gateway> &input);
	bool doUpdate(RelationWithData<Device, Gateway> &input);
	bool doUpdateAndActivate(RelationWithData<Device, Gateway> &input);

	bool doCreateProperty(RelationWithData<DeviceProperty, Device> &input);
	bool doUpdateProperty(RelationWithData<DeviceProperty, Device> &input);
	bool doRemoveProperty(Relation<const DeviceProperty, Device> &input);
	bool doFindProperty(Relation<DeviceProperty, Device> &input);
	void doListProperties(Relation<std::list<DeviceProperty>, Device> &input);

	bool prepareUpdate(RelationWithData<Device, Gateway> &input);
	bool tryActivateAndUpdate(Device &device,
			const Gateway &gateway, bool forceUpdate = false);

private:
	DeviceDao *m_dao;
	DevicePropertyDao *m_propertyDao;
	GatewayRPC *m_gatewayRPC;
	WorkScheduler *m_scheduler;
	DeviceAccessPolicy *m_policy;
};

}

#endif
