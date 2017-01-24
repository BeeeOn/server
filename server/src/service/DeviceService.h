#ifndef BEEEON_DEVICE_SERVICE_H
#define BEEEON_DEVICE_SERVICE_H

#include <list>
#include <vector>

#include "di/InjectorTarget.h"
#include "dao/Transactional.h"
#include "service/Single.h"
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
class DeviceService : public Transactional {
public:
	DeviceService();

	void setDeviceDao(DeviceDao *dao);
	void setGatewayRPC(GatewayRPC *rpc);
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

	bool unregister(Relation<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUnregister(input));
	}

	bool update(RelationWithData<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdate(input));
	}

	bool updateAndActivate(RelationWithData<Device, Gateway> &input)
	{
		return BEEEON_TRANSACTION_RETURN(bool, doUpdateAndActivate(input));
	}

protected:
	bool doFetch(Relation<Device, Gateway> &input);
	void doFetchMany(Single<std::list<Device>> &input);
	void doFetchMany(Relation<std::list<Device>, Gateway> &input);
	void doFetchActiveBy(Relation<std::vector<Device>, Gateway> &input);
	void doFetchInactiveBy(Relation<std::vector<Device>, Gateway> &input);

	bool doActivate(Relation<Device, Gateway> &input);
	bool doUnregister(Relation<Device, Gateway> &input);
	bool doUpdate(RelationWithData<Device, Gateway> &input);
	bool doUpdateAndActivate(RelationWithData<Device, Gateway> &input);

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
