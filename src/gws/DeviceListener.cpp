#include "gws/DeviceListener.h"

using namespace BeeeOn;

DeviceEvent::DeviceEvent()
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

DeviceListener::~DeviceListener()
{
}

void DeviceListener::onNewDevice(const DeviceEvent &e)
{
}

void DeviceListener::onRefusedNewDevice(const DeviceEvent &e)
{
}