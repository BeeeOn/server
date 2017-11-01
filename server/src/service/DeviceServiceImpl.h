#ifndef BEEEON_DEVICE_SERVICE_IMPL_H
#define BEEEON_DEVICE_SERVICE_IMPL_H

#include <list>
#include <vector>

#include "dao/DeviceDao.h"
#include "dao/DevicePropertyDao.h"
#include "policy/DeviceAccessPolicy.h"
#include "rpc/GatewayRPC.h"
#include "service/DeviceService.h"
#include "transaction/Transactional.h"
#include "work/WorkFacade.h"

namespace BeeeOn {

class DevicePropertyDao;

/**
 * Service for devices management.
 */
class DeviceServiceImpl : public DeviceService, public Transactional {
public:
	DeviceServiceImpl();

	void setDeviceDao(DeviceDao::Ptr dao);
	void setDevicePropertyDao(DevicePropertyDao::Ptr dao);
	void setGatewayRPC(GatewayRPC::Ptr rpc);
	void setWorkFacade(WorkFacade::Ptr facade);
	void setAccessPolicy(DeviceAccessPolicy::Ptr policy);

	bool fetch(Relation<Device, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchMany(Single<std::list<Device>> &input) override
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchMany(Relation<std::list<Device>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchActiveBy(Relation<std::vector<Device>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchActiveBy(input));
	}

	void fetchInactiveBy(Relation<std::vector<Device>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchInactiveBy(input));
	}

	bool activate(Relation<Device, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doActivate(input));
	}

	Work unregister(Relation<Device, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(Work, doUnregister(input));
	}

	bool update(RelationWithData<Device, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdate(input));
	}

	bool updateAndActivate(RelationWithData<Device, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdateAndActivate(input));
	}

	bool createProperty(RelationWithData<DeviceProperty, Device> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doCreateProperty(input));
	}

	bool updateProperty(RelationWithData<DeviceProperty, Device> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdateProperty(input));
	}

	bool removeProperty(Relation<const DeviceProperty, Device> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRemoveProperty(input));
	}

	bool findProperty(Relation<DeviceProperty, Device> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFindProperty(input));
	}

	void listProperties(Relation<std::list<DeviceProperty>, Device> &input) override
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
	Work doUnregister(Relation<Device, Gateway> &input);
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
	DeviceDao::Ptr m_dao;
	DevicePropertyDao::Ptr m_propertyDao;
	GatewayRPC::Ptr m_gatewayRPC;
	WorkFacade::Ptr m_workFacade;
	DeviceAccessPolicy::Ptr m_policy;
};

}

#endif

