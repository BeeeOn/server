#pragma once

#include <list>
#include <vector>

#include "dao/ControlDao.h"
#include "dao/DeviceDao.h"
#include "dao/DevicePropertyDao.h"
#include "dao/SensorHistoryDao.h"
#include "gws/DeviceListener.h"
#include "policy/DeviceAccessPolicy.h"
#include "model/DeviceExtended.h"
#include "provider/DeviceInfoProvider.h"
#include "rpc/GatewayRPC.h"
#include "service/DeviceService.h"
#include "service/GWSDeviceService.h"
#include "transaction/Transactional.h"
#include "util/CryptoConfig.h"
#include "util/EventSource.h"
#include "util/UnknownEvaluator.h"

namespace BeeeOn {

class DevicePropertyDao;

/**
 * Service for devices management.
 */
class DeviceServiceImpl : public DeviceService, public GWSDeviceService, public Transactional {
public:
	DeviceServiceImpl();

	void setDeviceDao(DeviceDao::Ptr dao);
	void setControlDao(ControlDao::Ptr dao);
	void setSensorHistoryDao(SensorHistoryDao::Ptr dao);
	void setDevicePropertyDao(DevicePropertyDao::Ptr dao);
	void setDeviceInfoProvider(DeviceInfoProvider::Ptr provider);
	void setGatewayRPC(GatewayRPC::Ptr rpc);
	void setAccessPolicy(DeviceAccessPolicy::Ptr policy);
	void setCryptoConfig(Poco::SharedPtr<CryptoConfig> config);
	void setEventsExecutor(AsyncExecutor::Ptr executor);
	void registerListener(DeviceListener::Ptr listener);

	bool fetch(Relation<Device, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doFetch(input));
	}

	void fetchMany(Single<std::list<Device>> &input) override
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchMany(Single<std::list<DeviceWithData>> &input) override
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchMany(Relation<std::list<Device>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchMany(Relation<std::list<DeviceWithData>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchMany(input));
	}

	void fetchActiveBy(Relation<std::vector<Device>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchActiveBy(input));
	}

	void fetchActiveBy(Relation<std::vector<DeviceWithData>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchActiveBy(input));
	}

	void fetchInactiveBy(Relation<std::vector<Device>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchInactiveBy(input));
	}

	void fetchInactiveBy(Relation<std::vector<DeviceWithData>, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doFetchInactiveBy(input));
	}

	bool activate(Relation<Device, Gateway> &input) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doActivate(input));
	}

	void unregister(Relation<Device, Gateway> &input) override
	{
		BEEEON_TRANSACTION(doUnregister(input));
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

	/**
	 * Remove all unused devices that are unpaired and have no history.
	 */
	void removeUnusedDevices();

	bool registerDevice(Device &device,
			const DeviceDescription &description,
			const Gateway &gateway) override
	{
		return BEEEON_TRANSACTION_RETURN(bool,
			doRegisterDevice(device, description, gateway));
	}

	void registerDeviceGroup(
			const std::vector<DeviceDescription> &descriptions,
			const Gateway &gateway) override
	{
		return BEEEON_TRANSACTION(
			doRegisterDeviceGroup(descriptions, gateway));
	}

	void fetchActiveWithPrefix(std::vector<DeviceExtended> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix) override
	{
		return BEEEON_TRANSACTION(doFetchActiveWithPrefix(devices, gateway, prefix));
	}

protected:
	void valuesFor(DeviceWithData &device);
	void propertiesFor(DeviceExtended &device);

	bool doFetch(Relation<Device, Gateway> &input);
	void doFetchMany(Single<std::list<Device>> &input);
	void doFetchMany(Single<std::list<DeviceWithData>> &input);
	void doFetchMany(Relation<std::list<Device>, Gateway> &input);
	void doFetchMany(Relation<std::list<DeviceWithData>, Gateway> &input);
	void doFetchActiveBy(Relation<std::vector<Device>, Gateway> &input);
	void doFetchActiveBy(Relation<std::vector<DeviceWithData>, Gateway> &input);
	void doFetchInactiveBy(Relation<std::vector<Device>, Gateway> &input);
	void doFetchInactiveBy(Relation<std::vector<DeviceWithData>, Gateway> &input);

	bool doActivate(Relation<Device, Gateway> &input);
	void doUnregister(Relation<Device, Gateway> &input);
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

	bool doRegisterFirst(Device &device,
			const DeviceDescription &description);
	bool doRegisterUpdate(Device &device,
			const DeviceDescription &description);
	bool doRegisterDevice(Device &device,
			const DeviceDescription &description,
			const Gateway &gateway);

	void doRegisterDeviceGroup(
			const std::vector<DeviceDescription> &descriptions,
			const Gateway &gateway);

	void doFetchActiveWithPrefix(std::vector<DeviceExtended> &devices,
			const Gateway &gateway,
			const DevicePrefix &prefix);

private:
	void syncDeviceProperties(
			Device &device,
			const DeviceDescription &description);

	Poco::SharedPtr<DeviceInfo> verifyDescription(
			const DeviceDescription &description) const;

	/**
	 * @brief Verify that count of given modules is less or equals to DeviceInfo
	 * specification and types of modules are identical.
	 *
	 * @throw InvalidArgumentException for invalid count of modules.
	 * @throw MultiException for invalid types of modules.
	 */
	void verifyModules(const Poco::SharedPtr<DeviceInfo> deviceInfo,
			const std::list<ModuleType> &modules) const;


private:
	DeviceDao::Ptr m_deviceDao;
	ControlDao::Ptr m_controlDao;
	SensorHistoryDao::Ptr m_historyDao;
	DevicePropertyDao::Ptr m_propertyDao;
	DeviceInfoProvider::Ptr m_deviceInfoProvider;
	GatewayRPC::Ptr m_gatewayRPC;
	DeviceAccessPolicy::Ptr m_policy;
	Poco::SharedPtr<CryptoConfig> m_cryptoConfig;
	Poco::SharedPtr<EventSource<DeviceListener>> m_eventSource;
	UnknownEvaluator m_unknownEvaluator;
};

}
