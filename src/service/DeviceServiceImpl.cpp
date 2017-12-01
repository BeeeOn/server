#include <Poco/Exception.h>
#include <Poco/DateTime.h>
#include <Poco/Logger.h>
#include <Poco/Nullable.h>

#include "di/Injectable.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "service/DeviceServiceImpl.h"
#include "work/DeviceUnpairWork.h"
#include "work/WorkFacade.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceServiceImpl)
BEEEON_OBJECT_CASTABLE(DeviceService)
BEEEON_OBJECT_REF("deviceDao", &DeviceServiceImpl::setDeviceDao)
BEEEON_OBJECT_REF("controlDao", &DeviceServiceImpl::setControlDao)
BEEEON_OBJECT_REF("sensorHistoryDao", &DeviceServiceImpl::setSensorHistoryDao)
BEEEON_OBJECT_REF("devicePropertyDao", &DeviceServiceImpl::setDevicePropertyDao)
BEEEON_OBJECT_REF("gatewayRPC", &DeviceServiceImpl::setGatewayRPC)
BEEEON_OBJECT_REF("accessPolicy", &DeviceServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("workFacade", &DeviceServiceImpl::setWorkFacade)
BEEEON_OBJECT_REF("transactionManager", &DeviceServiceImpl::setTransactionManager)
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

void DeviceServiceImpl::setControlDao(ControlDao::Ptr dao)
{
	m_controlDao = dao;
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

void DeviceServiceImpl::setWorkFacade(WorkFacade::Ptr facade)
{
	m_workFacade = facade;
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
	vector<ModuleInfo> sensors;
	list<Control> controls;
	size_t count = 0;

	for (const auto &info : *device.type()) {
		if (info.isControllable())
			controls.emplace_back(Control{info.id()});
		else
			sensors.emplace_back(info);

		count += 1;
	}

	// values of all modules
	vector<ValueAt> values(count);

	// a null result means that there is no data for that sensor yet
	vector<Nullable<ValueAt>> nullableValues;
	m_historyDao->fetchMany(device, sensors, nullableValues);

	size_t i = 0;
	for (const auto &sensor : sensors) {
		const auto &current = nullableValues.at(i++);
		const unsigned int index = sensor.id();

		if (!current.isNull())
			values[index] = current;

		// else leave the value as invalid
	}

	m_controlDao->fetchBy(controls, device);

	for (const auto &control : controls) {
		const unsigned int index = control.id();

		const auto &confirmed = control.lastConfirmed();
		if (!confirmed.isNull()) {
			const Control::State &state = confirmed;
			values[index] = {state.at().value(), state.value()};
		}
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

void DeviceServiceImpl::doFetchActiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_dao->fetchActiveBy(input.target(), input.base());
}

void DeviceServiceImpl::doFetchInactiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_GET,
			input, input.base());
	m_dao->fetchInactiveBy(input.target(), input.base());
}

Work DeviceServiceImpl::doUnregister(Relation<Device, Gateway> &input)
{
	m_policy->assure(DeviceAccessPolicy::ACTION_USER_UNREGISTER,
			input, input.target(), input.base());

	Device &device = input.target();

	if (!m_dao->fetch(device, input.base()))
		throw NotFoundException("no such device " + device);

	Work work(WorkID::random());
	DeviceUnpairWork content;
	content.setGatewayID(device.gateway().id());
	content.setDeviceID(device.id());
	work.setContent(content);

	m_workFacade->schedule(work, input);
	return work;
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
	if (!device.active()) {
		Device copy(device);

		m_gatewayRPC->pairDevice([copy, this](GatewayRPCResult::Ptr r) {
			switch (r->status()) {
			case GatewayRPCResult::Status::PENDING:
				logger().information(
					"device " + copy + " pairing is pending...",
					__FILE__, __LINE__);
				break;

			case GatewayRPCResult::Status::ACCEPTED:
				logger().debug(
					"device " + copy + " would be paired",
					__FILE__, __LINE__);
				break;

			case GatewayRPCResult::Status::SUCCESS:
				logger().information(
					"device " + copy + " successfully paired",
					__FILE__, __LINE__);
				break;

			case GatewayRPCResult::Status::FAILED:
			case GatewayRPCResult::Status::TIMEOUT:
			case GatewayRPCResult::Status::NOT_CONNECTED:
				logger().warning(
					"device " + copy + " failed to pair",
					__FILE__, __LINE__);
				break;
			}
		}, gateway, device);

		device.setActiveSince(DateTime());
		return m_dao->update(device, gateway);
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
