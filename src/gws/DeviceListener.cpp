#include "gws/DeviceListener.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

DeviceEvent::DeviceEvent()
{
}

DeviceEvent::DeviceEvent(const GatewayID &gatewayID, const DeviceID &deviceID):
	m_gatewayID(gatewayID),
	m_deviceID(deviceID)
{
}

void DeviceEvent::setGatewayID(const GatewayID &id)
{
	m_gatewayID = id;
}

const GatewayID &DeviceEvent::gatewayID() const
{
	return m_gatewayID;
}

void DeviceEvent::setDeviceID(const DeviceID &id)
{
	m_deviceID = id;
}

const DeviceID &DeviceEvent::deviceID() const
{
	return m_deviceID;
}

void DeviceEvent::setName(const string &name)
{
	m_name = name;
}

const string &DeviceEvent::name() const
{
	return m_name;
}

void DeviceEvent::setType(const DeviceInfo &type)
{
	m_type = type;
}

Nullable<DeviceInfo> DeviceEvent::type() const
{
	return m_type;
}

DeviceListener::~DeviceListener()
{
}

void DeviceListener::onNewDevice(const DeviceEvent &e)
{
}

void DeviceListener::onRefusedNewDevice(const DeviceEvent &e)
{
}

void DeviceListener::onPairRequested(const DeviceEvent &e)
{
}

void DeviceListener::onPairConfirmed(const DeviceEvent &e)
{
}

void DeviceListener::onPairFailed(const DeviceEvent &e)
{
}

void DeviceListener::onUnpairRequested(const DeviceEvent &e)
{
}

void DeviceListener::onUnpairConfirmed(const DeviceEvent &e)
{
}

void DeviceListener::onUnpairFailed(const DeviceEvent &e)
{
}
