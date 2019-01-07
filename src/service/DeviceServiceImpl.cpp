#include <set>

#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Nullable.h>
#include <Poco/Timestamp.h>

#include "di/Injectable.h"
#include "gws/DevicePairHandler.h"
#include "gws/DeviceUnpairHandler.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "service/DeviceServiceImpl.h"
#include "util/MultiException.h"
#include "util/ZipIterator.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceServiceImpl)
BEEEON_OBJECT_CASTABLE(DeviceService)
BEEEON_OBJECT_CASTABLE(GWSDeviceService)
BEEEON_OBJECT_PROPERTY("deviceDao", &DeviceServiceImpl::setDeviceDao)
BEEEON_OBJECT_PROPERTY("eventsExecutor", &DeviceServiceImpl::setEventsExecutor)
BEEEON_OBJECT_PROPERTY("sensorHistoryDao", &DeviceServiceImpl::setSensorHistoryDao)
BEEEON_OBJECT_PROPERTY("devicePropertyDao", &DeviceServiceImpl::setDevicePropertyDao)
BEEEON_OBJECT_PROPERTY("deviceInfoProvider", &DeviceServiceImpl::setDeviceInfoProvider)
BEEEON_OBJECT_PROPERTY("gatewayRPC", &DeviceServiceImpl::setGatewayRPC)
BEEEON_OBJECT_PROPERTY("accessPolicy", &DeviceServiceImpl::setAccessPolicy)
BEEEON_OBJECT_PROPERTY("transactionManager", &DeviceServiceImpl::setTransactionManager)
BEEEON_OBJECT_PROPERTY("cryptoConfig", &DeviceServiceImpl::setCryptoConfig)
BEEEON_OBJECT_PROPERTY("listeners", &DeviceServiceImpl::registerListener)
BEEEON_OBJECT_HOOK("done", &DeviceServiceImpl::removeUnusedDevices)
BEEEON_OBJECT_END(BeeeOn, DeviceServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceServiceImpl::DeviceServiceImpl():
	m_eventSource(new EventSource<DeviceListener>)
{
}

void DeviceServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_deviceDao = dao;
}

void DeviceServiceImpl::setSensorHistoryDao(SensorHistoryDao::Ptr dao)
{
	m_historyDao = dao;
}

void DeviceServiceImpl::setDevicePropertyDao(DevicePropertyDao::Ptr dao)
{
	m_propertyDao = dao;
}

void DeviceServiceImpl::setDeviceInfoProvider(DeviceInfoProvider::Ptr provider)
{
	m_deviceInfoProvider = provider;
}

void DeviceServiceImpl::setGatewayRPC(GatewayRPC::Ptr rpc)
{
	m_gatewayRPC = rpc;
}

void DeviceServiceImpl::setAccessPolicy(DeviceAccessPolicy::Ptr policy)
{
	m_policy = policy;
}

void DeviceServiceImpl::setCryptoConfig(SharedPtr<CryptoConfig> config)
{
	m_cryptoConfig = config;
}

void DeviceServiceImpl::setEventsExecutor(AsyncExecutor::Ptr executor)
{
	m_eventSource->setAsyncExecutor(executor);
}

void DeviceServiceImpl::registerListener(DeviceListener::Ptr listener)
{
	m_eventSource->addListener(listener);
}

bool DeviceServiceImpl::doFetch(Relation<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.target(), input.base());
	return m_deviceDao->fetch(input.target(), input.base());
}

void DeviceServiceImpl::valuesFor(DeviceWithData &device)
{
	// fetch last value of all device modules
	vector<ModuleInfo> modules;

	if (device.type().isNull()) {
		throw IllegalStateException(
			"device " + device + " does not have any known type");
	}

	for (const auto &info : *device.type())
		modules.emplace_back(info);

	// values of all modules
	vector<ValueAt> values(modules.size());

	// a null result means that there is no data for that module yet
	vector<Nullable<ValueAt>> nullableValues;
	m_historyDao->fetchMany(device, modules, nullableValues);

	size_t i = 0;
	for (const auto &module : modules) {
		const auto &current = nullableValues.at(i++);
		const unsigned int index = module.id();

		if (!current.isNull())
			values[index] = current;

		// else leave the value as invalid
	}

	device.setValues(values);
}

void DeviceServiceImpl::doFetchMany(Single<list<Device>> &input)
{
	m_policy->assureMany(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.target());

	list<Device> &devices = input.target();
	m_deviceDao->fetchMany(devices);
}

void DeviceServiceImpl::doFetchMany(Single<list<DeviceWithData>> &input)
{
	list<Device> devices;
	for (const auto &dev : input.target())
		devices.emplace_back(dev);

	m_deviceDao->fetchMany(devices);

	// convert to list of DeviceWithData
	list<DeviceWithData> result;
	for (const auto &dev : devices) {
		DeviceWithData device = dev;

		try {
			m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
					input, device, device.gateway());
		} catch (const InvalidAccessException &e) {
			continue;
		}

		try {
			valuesFor(device);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			continue; // skip failing devices
		}

		result.emplace_back(device);
	}

	input.target() = result;
}

void DeviceServiceImpl::doFetchMany(Relation<list<Device>, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET, input, input.base());

	list<Device> &devices = input.target();

	m_deviceDao->fetchMany(devices);

	list<Device>::iterator it = devices.begin();

	while (it != devices.end()) {
		Device &device = *it;

		try {
			m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
					input, device, input.base());
		} catch (const InvalidAccessException &e) {
			// drop inaccessible devices
			it = devices.erase(it);
			continue;
		}

		++it; // no erase occured, continue
	}
}

/**
 * The method performs 1 + 2 * N Dao requests where N is the number of devices.
 * The first query obtains list of Device instances. Because we need a list
 * of DeviceWithData instances, the loop would convert it. The conversion
 * fetches module data for every single device.
 *
 * The method should be optimized (moved to Dao layer) if needed.
 */
void DeviceServiceImpl::doFetchMany(Relation<list<DeviceWithData>, Gateway> &input)
{
	// fetch list of Devices
	list<Device> devices;
	for (const auto &dev : input.target())
		devices.emplace_back(dev);

	m_policy->assureMany(DeviceAccessPolicy::ACTION_USER_GET, input, devices);

	m_deviceDao->fetchMany(devices);

	// convert to list of DeviceWithData
	list<DeviceWithData> result;
	for (const auto &dev : devices) {
		DeviceWithData device = dev;

		try {
			m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
					input, device, input.base());
		} catch (const InvalidAccessException &e) {
			continue;
		}

		try {
			valuesFor(device);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			continue; // skip failing devices
		}

		result.emplace_back(device);
	}

	input.target() = result;
}

void DeviceServiceImpl::doFetchActiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_deviceDao->fetchActiveBy(input.target(), input.base());
}

void DeviceServiceImpl::doFetchActiveBy(Relation<vector<DeviceWithData>, Gateway> &input)
{
	// fetch list of Devices
	vector<Device> devices;
	for (const auto &dev : input.target())
		devices.emplace_back(dev);

	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_deviceDao->fetchActiveBy(devices, input.base());

	// convert to list of DeviceWithData
	vector<DeviceWithData> result;
	for (const auto &dev : devices) {
		DeviceWithData device = dev;

		try {
			m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
					input, device, input.base());
		} catch (const InvalidAccessException &e) {
			continue;
		}

		try {
			valuesFor(device);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			continue; // skip failing devices
		}

		result.emplace_back(device);
	}

	input.target() = result;
}

void DeviceServiceImpl::doFetchInactiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_deviceDao->fetchInactiveBy(input.target(), input.base());
}

void DeviceServiceImpl::doFetchInactiveBy(Relation<vector<DeviceWithData>, Gateway> &input)
{
	// fetch list of Devices
	vector<Device> devices;
	for (const auto &dev : input.target())
		devices.emplace_back(dev);

	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_deviceDao->fetchInactiveBy(devices, input.base());

	// convert to list of DeviceWithData
	vector<DeviceWithData> result;
	for (const auto &dev : devices) {
		DeviceWithData device = dev;

		try {
			m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
					input, device, input.base());
		} catch (const InvalidAccessException &e) {
			continue;
		}

		try {
			valuesFor(device);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			continue; // skip failing devices
		}

		result.emplace_back(device);
	}

	input.target() = result;

}

void DeviceServiceImpl::doUnregister(Relation<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_UNREGISTER,
			input, input.target(), input.base());

	Device &device = input.target();

	if (!m_deviceDao->fetch(device, input.base()))
		throw NotFoundException("no such device " + device);

	if (!device.status().active())
		return;

	device.status().setState(DeviceStatus::STATE_INACTIVE_PENDING);
	device.status().setLastChanged({});

	if (!m_deviceDao->update(device, input.base()))
		throw NotFoundException("device " + device + " seems to not exist");

	DeviceUnpairHandler::Ptr handler = new DeviceUnpairHandler(device, m_deviceDao, m_eventSource);
	handler->setTransactionManager(transactionManager());

	m_gatewayRPC->unpairDevice(handler, input.base(), device);
}

bool DeviceServiceImpl::doActivate(Relation<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_ACTIVATE,
			input, input.target(), input.base());

	Device &device = input.target();

	if (!m_deviceDao->fetch(device, input.base()))
		return false;

	return tryActivateAndUpdate(device, input.base());
}

bool DeviceServiceImpl::tryActivateAndUpdate(Device &device,
		const Gateway &gateway, bool forceUpdate)
{
	const DeviceStatus &status = device.status();

	if (!status.active()) {
		device.status().setState(DeviceStatus::STATE_ACTIVE_PENDING);
		device.status().setLastChanged({});

		if (!m_deviceDao->update(device, gateway))
			throw NotFoundException("device " + device + " seems to not exist");

		DevicePairHandler::Ptr handler = new DevicePairHandler(device, m_deviceDao, m_eventSource);
		handler->setTransactionManager(transactionManager());

		m_gatewayRPC->pairDevice(handler, gateway, device);
		return true;
	}

	return forceUpdate? m_deviceDao->update(device, gateway) : false;
}

bool DeviceServiceImpl::prepareUpdate(RelationWithData<Device, Gateway> &input)
{
	if (!m_deviceDao->fetch(input.target(), input.base()))
		return false;

	input.data().partial(input.target());
	return true;
}

bool DeviceServiceImpl::doUpdate(RelationWithData<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_UPDATE,
			input, input.target(), input.base());

	if (!prepareUpdate(input))
		return false;

	return m_deviceDao->update(input.target(), input.base());
}

bool DeviceServiceImpl::doUpdateAndActivate(
		RelationWithData<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_UPDATE_AND_ACTIVATE,
			input, input.target(), input.base());

	if (!prepareUpdate(input))
		return false;

	return tryActivateAndUpdate(input.target(), input.base(), true);
}

bool DeviceServiceImpl::doCreateProperty(RelationWithData<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_UPDATE,
			input, input.base(), gateway);

	input.data().full(input.target());

	DeviceProperty duplicate(input.target());
	if (m_propertyDao->fetch(duplicate, input.base()))
		return false; // would not be inserted

	return m_propertyDao->insert(input.target(), input.base());
}

bool DeviceServiceImpl::doUpdateProperty(RelationWithData<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_UPDATE,
			input, input.base(), gateway);

	if (!m_propertyDao->fetch(input.target(), input.base()))
		return false;

	input.data().partial(input.target());

	return m_propertyDao->update(input.target(), input.base());
}

bool DeviceServiceImpl::doRemoveProperty(Relation<const DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_UPDATE,
			input, input.base(), gateway);

	return m_propertyDao->remove(input.target(), input.base());
}

bool DeviceServiceImpl::doFindProperty(Relation<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base(), gateway);

	return m_propertyDao->fetch(input.target(), input.base());
}

void DeviceServiceImpl::doListProperties(Relation<list<DeviceProperty>, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base(), gateway);

	return m_propertyDao->fetchByDevice(input.target(), input.base());
}

void DeviceServiceImpl::removeUnusedDevices()
{
	size_t count = BEEEON_TRANSACTION_RETURN(size_t,
                               m_deviceDao->removeUnused());

	if (count > 0) {
		logger().warning("removed "
				+ to_string(count)
				+ " unused devices",
			__FILE__, __LINE__);
	}
}

SharedPtr<DeviceInfo> DeviceServiceImpl::verifyDescription(
		const DeviceDescription &description) const
{
	auto type = m_deviceInfoProvider->findByNameAndVendor(
		description.productName(),
		description.vendor());
	if (type.isNull()) {
		throw NotFoundException("no such device type for "
			"'" + description.toString() + "' specification");
	}

	verifyModules(type, description.dataTypes());
	return type;
}

bool DeviceServiceImpl::doRegisterFirst(
		Device &device,
		const DeviceDescription &description)
{
	if (!description.name().empty())
		device.setName(description.name());
	else
		device.setName(description.productName());

	device.setType(verifyDescription(description));

	DeviceStatus &status = device.status();

	status.setFirstSeen(Timestamp());
	status.setLastSeen(Timestamp());
	status.setState(DeviceStatus::STATE_INACTIVE);
	status.setLastChanged({});

	return m_deviceDao->insert(device, device.gateway());
}

bool DeviceServiceImpl::doRegisterUpdate(
		Device &device,
		const DeviceDescription &description)
{
	auto type = verifyDescription(description);

	if (type != device.type()) {
		throw IllegalStateException(
			"description " + description.toString()
			+ " has non-matching device type for device " + device);
	}

	if (!description.name().empty())
		device.setName(description.name());

	device.status().setLastSeen(Timestamp());

	return m_deviceDao->update(device, device.gateway());
}

bool DeviceServiceImpl::doRegisterDevice(Device &device,
		const DeviceDescription &description,
		const Gateway &gateway)
{
	if (m_deviceDao->fetch(device, gateway)) {
		 if (!doRegisterUpdate(device, description))
			 return false;
	}
	else {
		device.setGateway(gateway);
		if (!doRegisterFirst(device, description))
			return false;
	}

	if (m_cryptoConfig.isNull())
		return true;

	syncDeviceProperties(device, description);
	return true;
}

static Nullable<string> extractValue(
		const DevicePropertyKey &key,
		const DeviceDescription &description)
{
	switch (key) {
	case DevicePropertyKey::KEY_IP_ADDRESS:
		if (description.ipAddress().isNull())
			return {};

		return description.ipAddress().value().toString();

	case DevicePropertyKey::KEY_FIRMWARE:
		if (description.firmware().empty())
			return {};

		return description.firmware();

	case DevicePropertyKey::KEY_MAC_ADDRESS:
		if (description.macAddress().isNull())
			return {};

		return description.macAddress().value(':').toString();

	case DevicePropertyKey::KEY_SERIAL_NUMBER:
		if (description.serialNumber().isNull())
			return {};

		return to_string(description.serialNumber().value());

	default:
		return {};
	}
}

void DeviceServiceImpl::syncDeviceProperties(
		Device &device,
		const DeviceDescription &description)
{
	list<DeviceProperty> properties;
	m_propertyDao->fetchByDevice(properties, device);
	set<DevicePropertyKey> processedKeys;

	for (auto &property : properties) {
		if (!property.key().isGatewayWritable())
			continue;

		processedKeys.emplace(property.key());
		const auto input = extractValue(property.key(), description);

		if (input.isNull())
			continue;

		property.setFromString(input.value(), *m_cryptoConfig);

		try {
			if (!m_propertyDao->update(property, device)) {
				logger().warning(
					"failed to update property "
					+ property.key().toString()
					+ " to " + description.firmware()
					+ " (race condition?)",
					__FILE__, __LINE__);
			}
		}
		BEEEON_CATCH_CHAIN(logger())
	}

	for (const auto &key : DevicePropertyKey::all()) {
		if (!key.isGatewayWritable())
			continue;

		if (processedKeys.find(key) != end(processedKeys))
			continue;

		DeviceProperty property;
		const auto input = extractValue(key, description);

		if (input.isNull())
			continue;

		property.setKey(key);
		property.setParams(key.deriveParams(*m_cryptoConfig));
		property.setFromString(input.value(), *m_cryptoConfig);

		try {
			if (!m_propertyDao->insert(property, device)) {
				logger().warning(
					"failed to insert property "
					+ property.key().toString()
					+ " to " + description.firmware()
					+ " (race condition?)",
					__FILE__, __LINE__);
			}
		}
		BEEEON_CATCH_CHAIN(logger())
	}
}

void DeviceServiceImpl::doRegisterDeviceGroup(
		const vector<DeviceDescription> &descriptions,
		const Gateway &gateway)
{
	for (auto &des : descriptions) {
		Device device(des.id());
		device.setRefresh(des.refreshTime());

		if (!doRegisterDevice(device, des, gateway)) {
			throw IllegalStateException("registration of device "
				"'" + des.toString() + "' failed");
		}
	}
}

void DeviceServiceImpl::propertiesFor(DeviceExtended &device)
{
	list<DeviceProperty> properties;

	m_propertyDao->fetchByDevice(properties, device);
	device.setProperties(properties);
}

void DeviceServiceImpl::doFetchActiveWithPrefix(vector<DeviceExtended> &devices,
		const Gateway &gateway,
		const DevicePrefix &prefix)
{
	vector<Device> simple;
	m_deviceDao->fetchActiveWithPrefix(simple, gateway, prefix);

	for (const auto &one : simple) {
		DeviceExtended device = one;

		try {
			valuesFor(device);
			propertiesFor(device);

			SharedPtr<DeviceInfo> info = device.type();
			if (info.isNull())
				continue;

			const auto &values = device.values();
			Zip<const set<ModuleInfo>, const vector<ValueAt>> zip(
				info->modules(), values);
			vector<ValueAt> raw;

			for (const auto &pair : zip) {
				const auto &info = pair.first;
				auto value = pair.second;

				if (value.isValid() && !info.toUnknown().empty()) {
					value.setValue(m_unknownEvaluator.toRaw(
							info, value.value()));
					raw.emplace_back(value);
				}
				else {
					raw.emplace_back(value);
				}
			}

			device.setValues(raw);
		}
		catch (const Exception &e) {
			logger().log(e, __FILE__, __LINE__);
			continue;
		}

		devices.emplace_back(device);
	}
}

void DeviceServiceImpl::verifyModules(
		const SharedPtr<DeviceInfo> deviceInfo,
		const list<ModuleType> &modules) const
{
	if (modules.size() > deviceInfo->modules().size()) {
		throw InvalidArgumentException("invalid count of modules "
			+ to_string(modules.size()) + " for device " + *deviceInfo);
	}

	MultiException ex;

	Zip<const set<ModuleInfo>, const list<ModuleType>> zip(
		deviceInfo->modules(), modules);

	auto i = 0;
	auto it = zip.begin();
	for (; it != zip.end(); ++it, ++i) {
		const auto &expect = (*it).first;
		const auto &given = (*it).second;

		try {
			expect.assureCompatible(given);
		}
		catch (const Exception &e) {
			ex.caught(e);
		}
	}

	string missing;

	for (auto expectIt = it.firstIterator(); expectIt != zip.firstEnd(); ++expectIt, ++i) {
		if (!missing.empty())
			missing += ", ";

		missing += to_string(i) + " " + expectIt->type()->name();
	}

	if (!missing.empty()) {
		logger().warning("missing specification of " + missing
			 + " for device " + *deviceInfo);
	}

	if (!ex.empty())
		ex.rethrow();
}
