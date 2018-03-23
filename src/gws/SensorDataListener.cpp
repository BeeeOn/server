#include "gws/SensorDataListener.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

SensorDataEvent::SensorDataEvent()
{
}

void SensorDataEvent::setGatewayID(const GatewayID &id)
{
	m_gatewayID = id;
}

const GatewayID &SensorDataEvent::gatewayID() const
{
	return m_gatewayID;
}

void SensorDataEvent::setDeviceID(const DeviceID &id)
{
	m_deviceID = id;
}

const DeviceID &SensorDataEvent::deviceID() const
{
	return m_deviceID;
}

void SensorDataEvent::setStamp(const Timestamp &stamp)
{
	m_stamp = stamp;
}

const Timestamp &SensorDataEvent::stamp() const
{
	return m_stamp;
}

void SensorDataEvent::setData(const vector<ModuleValue> &data)
{
	m_data = data;
}

const vector<ModuleValue> &SensorDataEvent::data() const
{
	return m_data;
}

SensorDataListener::~SensorDataListener()
{
}

void SensorDataListener::onReceived(const SensorDataEvent &e)
{
}
