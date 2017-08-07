#include <Poco/Exception.h>
#include <Poco/DateTime.h>
#include <Poco/Logger.h>
#include <Poco/Nullable.h>

#include "dao/DeviceDao.h"
#include "dao/DevicePropertyDao.h"
#include "di/Injectable.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "rpc/GatewayRPC.h"
#include "service/DeviceServiceImpl.h"
#include "work/DeviceUnpairWork.h"
#include "work/WorkFacade.h"

BEEEON_OBJECT_BEGIN(BeeeOn, DeviceServiceImpl)
BEEEON_OBJECT_CASTABLE(DeviceService)
BEEEON_OBJECT_REF("deviceDao", &DeviceServiceImpl::setDeviceDao)
BEEEON_OBJECT_REF("devicePropertyDao", &DeviceServiceImpl::setDevicePropertyDao)
BEEEON_OBJECT_REF("gatewayRPC", &DeviceServiceImpl::setGatewayRPC)
BEEEON_OBJECT_REF("accessPolicy", &DeviceServiceImpl::setAccessPolicy)
BEEEON_OBJECT_REF("workFacade", &DeviceServiceImpl::setWorkFacade)
BEEEON_OBJECT_REF("transactionManager", &DeviceServiceImpl::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, DeviceServiceImpl)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceServiceImpl::DeviceServiceImpl():
	m_dao(&NullDeviceDao::instance()),
	m_propertyDao(&NullDevicePropertyDao::instance()),
	m_gatewayRPC(&NullGatewayRPC::instance()),
	m_workFacade(&NullWorkFacade::instance())
{
}

void DeviceServiceImpl::setDeviceDao(DeviceDao *dao)
{
	m_dao = dao? dao : &NullDeviceDao::instance();
}

void DeviceServiceImpl::setDevicePropertyDao(DevicePropertyDao *dao)
{
	m_propertyDao = dao? dao : &NullDevicePropertyDao::instance();
}

void DeviceServiceImpl::setGatewayRPC(GatewayRPC *rpc)
{
	m_gatewayRPC = rpc? rpc : &NullGatewayRPC::instance();
}

void DeviceServiceImpl::setWorkFacade(WorkFacade *facade)
{
	m_workFacade = facade? facade : &NullWorkFacade::instance();
}

void DeviceServiceImpl::setAccessPolicy(DeviceAccessPolicy::Ptr policy)
{
	m_policy = policy;
}

bool DeviceServiceImpl::doFetch(Relation<Device, Gateway> &input)
{
	m_policy->assureGet(input, input.target(), input.base());
	return m_dao->fetch(input.target(), input.base());
}

void DeviceServiceImpl::doFetchMany(Single<list<Device>> &input)
{
	m_policy->assureGetMany(input, input.target());

	list<Device> &devices = input.target();
	m_dao->fetchMany(devices);
}


void DeviceServiceImpl::doFetchMany(Relation<list<Device>, Gateway> &input)
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

void DeviceServiceImpl::doFetchActiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assureListActiveDevices(input, input.base());
	m_dao->fetchActiveBy(input.target(), input.base());
}

void DeviceServiceImpl::doFetchInactiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_policy->assureListInactiveDevices(input, input.base());
	m_dao->fetchInactiveBy(input.target(), input.base());
}

Work DeviceServiceImpl::doUnregister(Relation<Device, Gateway> &input)
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

bool DeviceServiceImpl::doActivate(Relation<Device, Gateway> &input)
{
	m_policy->assureActivate(input, input.target(), input.base());

	Device &device = input.target();

	if (!m_dao->fetch(device, input.base()))
		return false;

	return tryActivateAndUpdate(device, input.base());
}

bool DeviceServiceImpl::tryActivateAndUpdate(Device &device,
		const Gateway &gateway, bool forceUpdate)
{
	if (!device.active()) {
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
	m_policy->assureUpdate(input, input.target(), input.base());

	if (!prepareUpdate(input))
		return false;

	return m_dao->update(input.target(), input.base());
}

bool DeviceServiceImpl::doUpdateAndActivate(
		RelationWithData<Device, Gateway> &input)
{
	m_policy->assureUpdateAndActivate(input,
			input.target(), input.base());

	if (!prepareUpdate(input))
		return false;

	return tryActivateAndUpdate(input.target(), input.base(), true);
}

bool DeviceServiceImpl::doCreateProperty(RelationWithData<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureUpdate(input, input.base(), gateway);

	input.data().full(input.target());

	return m_propertyDao->insert(input.target(), input.base());
}

bool DeviceServiceImpl::doUpdateProperty(RelationWithData<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureUpdate(input, input.base(), gateway);

	if (!m_propertyDao->fetch(input.target(), input.base()))
		return false;

	input.data().partial(input.target());

	return m_propertyDao->update(input.target(), input.base());
}

bool DeviceServiceImpl::doRemoveProperty(Relation<const DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureUpdate(input, input.base(), gateway);

	return m_propertyDao->remove(input.target(), input.base());
}

bool DeviceServiceImpl::doFindProperty(Relation<DeviceProperty, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureGet(input, input.base(), gateway);

	return m_propertyDao->fetch(input.target(), input.base());
}

void DeviceServiceImpl::doListProperties(Relation<list<DeviceProperty>, Device> &input)
{
	const Gateway &gateway = input.base().gateway();
	m_policy->assureGet(input, input.base(), gateway);

	return m_propertyDao->fetchByDevice(input.target(), input.base());
}
