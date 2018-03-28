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

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceServiceImpl)
BEEEON_OBJECT_CASTABLE(DeviceService)
BEEEON_OBJECT_PROPERTY("deviceDao", &DeviceServiceImpl::setDeviceDao)
BEEEON_OBJECT_PROPERTY("sensorHistoryDao", &DeviceServiceImpl::setSensorHistoryDao)
BEEEON_OBJECT_PROPERTY("devicePropertyDao", &DeviceServiceImpl::setDevicePropertyDao)
BEEEON_OBJECT_PROPERTY("gatewayRPC", &DeviceServiceImpl::setGatewayRPC)
BEEEON_OBJECT_PROPERTY("accessPolicy", &DeviceServiceImpl::setAccessPolicy)
BEEEON_OBJECT_PROPERTY("transactionManager", &DeviceServiceImpl::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, DeviceServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceServiceImpl::DeviceServiceImpl()
{
}

void DeviceServiceImpl::setDeviceDao(DeviceDao::Ptr dao)
{
	m_dao = dao;
}

void DeviceServiceImpl::setSensorHistoryDao(SensorHistoryDao::Ptr dao)
{
	m_historyDao = dao;
}

void DeviceServiceImpl::setDevicePropertyDao(DevicePropertyDao::Ptr dao)
{
	m_propertyDao = dao;
}

void DeviceServiceImpl::setGatewayRPC(GatewayRPC::Ptr rpc)
{
	m_gatewayRPC = rpc;
}

void DeviceServiceImpl::setAccessPolicy(DeviceAccessPolicy::Ptr policy)
{
	m_policy = policy;
}

bool DeviceServiceImpl::doFetch(Relation<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.target(), input.base());
	return m_dao->fetch(input.target(), input.base());
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
	m_dao->fetchMany(devices);
}

void DeviceServiceImpl::doFetchMany(Single<list<DeviceWithData>> &input)
{
	list<Device> devices;
	for (const auto &dev : input.target())
		devices.emplace_back(dev);

	m_dao->fetchMany(devices);

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

	m_dao->fetchMany(devices);

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

	m_dao->fetchMany(devices);

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
	m_dao->fetchActiveBy(input.target(), input.base());
}

void DeviceServiceImpl::doFetchActiveBy(Relation<vector<DeviceWithData>, Gateway> &input)
{
	// fetch list of Devices
	vector<Device> devices;
	for (const auto &dev : input.target())
		devices.emplace_back(dev);

	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_dao->fetchActiveBy(devices, input.base());

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
	m_dao->fetchInactiveBy(input.target(), input.base());
}

void DeviceServiceImpl::doFetchInactiveBy(Relation<vector<DeviceWithData>, Gateway> &input)
{
	// fetch list of Devices
	vector<Device> devices;
	for (const auto &dev : input.target())
		devices.emplace_back(dev);

	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_dao->fetchInactiveBy(devices, input.base());

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

	if (!m_dao->fetch(device, input.base()))
		throw NotFoundException("no such device " + device);

	if (!device.status().active())
		return;

	device.status().setState(DeviceStatus::STATE_INACTIVE_PENDING);
	device.status().setLastChanged({});

	if (!m_dao->update(device, input.base()))
		throw NotFoundException("device " + device + " seems to not exist");

	DeviceUnpairHandler::Ptr handler = new DeviceUnpairHandler(device, m_dao);
	handler->setTransactionManager(transactionManager());

	m_gatewayRPC->unpairDevice(handler, input.base(), device);
}

bool DeviceServiceImpl::doActivate(Relation<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_ACTIVATE,
			input, input.target(), input.base());

	Device &device = input.target();

	if (!m_dao->fetch(device, input.base()))
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

		if (!m_dao->update(device, gateway))
			throw NotFoundException("device " + device + " seems to not exist");

		DevicePairHandler::Ptr handler = new DevicePairHandler(device, m_dao);
		handler->setTransactionManager(transactionManager());

		m_gatewayRPC->pairDevice(handler, gateway, device);
		return true;
	}

	return forceUpdate? m_dao->update(device, gateway) : false;
}

bool DeviceServiceImpl::prepareUpdate(RelationWithData<Device, Gateway> &input)
{
	if (!m_dao->fetch(input.target(), input.base()))
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

	return m_dao->update(input.target(), input.base());
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
