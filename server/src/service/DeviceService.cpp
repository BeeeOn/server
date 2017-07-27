#include <Poco/Exception.h>
#include <Poco/DateTime.h>
#include <Poco/Logger.h>
#include <Poco/Nullable.h>

#include "dao/DeviceDao.h"
#include "dao/DevicePropertyDao.h"
#include "di/Injectable.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "policy/DeviceAccessPolicy.h"
#include "rpc/GatewayRPC.h"
#include "service/DeviceService.h"
#include "work/DeviceUnpairWork.h"
#include "work/WorkFacade.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceService)
BEEEON_OBJECT_REF("deviceDao", &DeviceService::setDeviceDao)
BEEEON_OBJECT_REF("devicePropertyDao", &DeviceService::setDevicePropertyDao)
BEEEON_OBJECT_REF("gatewayRPC", &DeviceService::setGatewayRPC)
BEEEON_OBJECT_REF("accessPolicy", &DeviceService::setAccessPolicy)
BEEEON_OBJECT_REF("workFacade", &DeviceService::setWorkFacade)
BEEEON_OBJECT_REF("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, DeviceService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceService::DeviceService():
	m_dao(&NullDeviceDao::instance()),
	m_propertyDao(&NullDevicePropertyDao::instance()),
	m_gatewayRPC(&NullGatewayRPC::instance()),
	m_workFacade(&NullWorkFacade::instance()),
	m_policy(&NullDeviceAccessPolicy::instance())
{
}

void DeviceService::setDeviceDao(DeviceDao *dao)
{
	m_dao = dao? dao : &NullDeviceDao::instance();
}

void DeviceService::setDevicePropertyDao(DevicePropertyDao *dao)
{
	m_propertyDao = dao? dao : &NullDevicePropertyDao::instance();
}

void DeviceService::setGatewayRPC(GatewayRPC *rpc)
{
	m_gatewayRPC = rpc? rpc : &NullGatewayRPC::instance();
}

void DeviceService::setWorkFacade(WorkFacade *facade)
{
	m_workFacade = facade? facade : &NullWorkFacade::instance();
}

void DeviceService::setAccessPolicy(DeviceAccessPolicy *policy)
{
	m_policy = policy? policy : &NullDeviceAccessPolicy::instance();
}

bool DeviceService::doFetch(Relation<Device, Gateway> &input)
{
	m_policy->assureGet(input, input.target(), input.base());
	return m_dao->fetch(input.target(), input.base());
}

void DeviceService::doFetchMany(Single<list<Device>> &input)
{
	m_policy->assureGetMany(input, input.target());

	list<Device> &devices = input.target();
	m_dao->fetchMany(devices);
}


void DeviceService::doFetchMany(Relation<list<Device>, Gateway> &input)
{
	list<Device> &devices = input.target();
	list<Device>::iterator it = devices.begin();

	// TODO: move to Dao layer
	while (it != devices.end()) {
		Device &device = *it;

		try {
			m_policy->assureGet(input, device, input.base());

			if (!m_dao->fetch(device, input.base())) {
				it = devices.erase(it);
				continue;
			}

		} catch (const InvalidAccessException &e) {
			// drop inaccessible devices
			it = devices.erase(it);
			continue;
		}

		++it; // no erase occured, continue
	}
}

void DeviceService::doFetchActiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assureListActiveDevices(input, input.base());
	m_dao->fetchActiveBy(input.target(), input.base());
}

void DeviceService::doFetchInactiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assureListInactiveDevices(input, input.base());
	m_dao->fetchInactiveBy(input.target(), input.base());
}

Work DeviceService::doUnregister(Relation<Device, Gateway> &input)
{
	m_policy->assureUnregister(input, input.target(), input.base());

	Device &device = input.target();

	if (!m_dao->fetch(device, input.base()))
		throw NotFoundException("no such device " + device);

	Work work(WorkID::random());
	DeviceUnpairWork content;
	content.setGatewayID(device.gateway().id());
	content.setDeviceID(device.id());
	work.setContent(content);

	m_workFacade->schedule(work);
	return work;
}

bool DeviceService::doActivate(Relation<Device, Gateway> &input)
{
	m_policy->assureActivate(input, input.target(), input.base());

	Device &device = input.target();

	if (!m_dao->fetch(device, input.base()))
		return false;

	return tryActivateAndUpdate(device, input.base());
}

bool DeviceService::tryActivateAndUpdate(Device &device,
		const Gateway &gateway, bool forceUpdate)
{
	if (!device.active()) {
		device.setActiveSince(DateTime());
		return m_dao->update(device, gateway);
	}

	return forceUpdate? m_dao->update(device, gateway) : false;
}

bool DeviceService::prepareUpdate(RelationWithData<Device, Gateway> &input)
{
	if (!m_dao->fetch(input.target(), input.base()))
		return false;

	input.data().partial(input.target());
	return true;
}

bool DeviceService::doUpdate(RelationWithData<Device, Gateway> &input)
{
	m_policy->assureUpdate(input, input.target(), input.base());

	if (!prepareUpdate(input))
		return false;

	return m_dao->update(input.target(), input.base());
}

bool DeviceService::doUpdateAndActivate(
		RelationWithData<Device, Gateway> &input)
{
	m_policy->assureUpdateAndActivate(input,
			input.target(), input.base());

	if (!prepareUpdate(input))
		return false;

	return tryActivateAndUpdate(input.target(), input.base(), true);
}

bool DeviceService::doCreateProperty(RelationWithData<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureUpdate(input, input.base(), gateway);

	input.data().full(input.target());

	return m_propertyDao->insert(input.target(), input.base());
}

bool DeviceService::doUpdateProperty(RelationWithData<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureUpdate(input, input.base(), gateway);

	if (!m_propertyDao->fetch(input.target(), input.base()))
		return false;

	input.data().partial(input.target());

	return m_propertyDao->update(input.target(), input.base());
}

bool DeviceService::doRemoveProperty(Relation<const DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureUpdate(input, input.base(), gateway);

	return m_propertyDao->remove(input.target(), input.base());
}

bool DeviceService::doFindProperty(Relation<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureGet(input, input.base(), gateway);

	return m_propertyDao->fetch(input.target(), input.base());
}

void DeviceService::doListProperties(Relation<list<DeviceProperty>, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureGet(input, input.base(), gateway);

	return m_propertyDao->fetchByDevice(input.target(), input.base());
}
