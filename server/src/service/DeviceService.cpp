#include <Poco/Exception.h>
#include <Poco/Logger.h>
#include <Poco/Nullable.h>
#include <Poco/DateTime.h>

#include "service/DeviceService.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "dao/DeviceDao.h"
#include "rpc/GatewayRPC.h"
#include "Debug.h"

BEEEON_OBJECT(DeviceService, BeeeOn::DeviceService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceService::DeviceService():
	m_dao(&NullDeviceDao::instance()),
	m_gatewayRPC(&NullGatewayRPC::instance())
{
	injector<DeviceService, DeviceDao>("deviceDao",
			&DeviceService::setDeviceDao);
	injector<DeviceService, GatewayRPC>("gatewayRPC",
			&DeviceService::setGatewayRPC);
}

void DeviceService::setDeviceDao(DeviceDao *dao)
{
	m_dao = dao? dao : &NullDeviceDao::instance();
}

void DeviceService::setGatewayRPC(GatewayRPC *rpc)
{
	m_gatewayRPC = rpc? rpc : &NullGatewayRPC::instance();
}

bool DeviceService::fetch(Relation<Device, Gateway> &input)
{
	TRACE_METHOD();

	return m_dao->fetch(input.target(), input.base());
}

void DeviceService::fetchMany(Relation<list<Device>, Gateway> &input)
{
	list<Device> &devices = input.target();
	list<Device>::iterator it = devices.begin();

	// TODO: move to Dao layer
	while (it != devices.end()) {
		Device &device = *it;

		if (!m_dao->fetch(device, input.base()))
			it = devices.erase(it);
		else
			++it;
	}
}

void DeviceService::fetchActiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_dao->fetchActiveBy(input.target(), input.base());
}

void DeviceService::fetchInactiveBy(Relation<vector<Device>, Gateway> &input)
{
	m_dao->fetchInactiveBy(input.target(), input.base());
}

bool DeviceService::unregister(Relation<Device, Gateway> &input)
{
	try {
		m_gatewayRPC->unpairDevice(input.base(), input.target());
		return true;
	}
	catch (const Poco::Exception &e) {
		LOGGER_CLASS(this).log(e, __FILE__, __LINE__);
		return false;
	}
}

bool DeviceService::activate(Relation<Device, Gateway> &input)
{
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

bool DeviceService::update(RelationWithData<Device, Gateway> &input)
{
	if (!prepareUpdate(input))
		return false;

	return m_dao->update(input.target(), input.base());
}

bool DeviceService::updateAndActivate(
		RelationWithData<Device, Gateway> &input)
{
	if (!prepareUpdate(input))
		return false;

	return tryActivateAndUpdate(input.target(), input.base(), true);
}
