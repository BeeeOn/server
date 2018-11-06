#include <Poco/Timespan.h>

#include "model/Device.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Device::Device()
{
}

Device::Device(const ID &id):
	Entity(id)
{
}

void Device::setGateway(const Gateway &gateway)
{
	m_gateway = gateway;
}

const Gateway &Device::gateway() const
{
	return m_gateway;
}

void Device::setLocation(const Location &location)
{
	m_location = location;
}

const Location &Device::location() const
{
	return m_location;
}

void Device::setName(const string &name)
{
	m_name = name;
}

const string &Device::name() const
{
	return m_name;
}

void Device::setType(const SharedPtr<DeviceInfo> type)
{
	m_type = type;
}

const SharedPtr<DeviceInfo> Device::type() const
{
	return m_type;
}

void Device::setRefresh(const RefreshTime &refresh)
{
	m_refresh = refresh;
}

const RefreshTime &Device::refresh() const
{
	return m_refresh;
}

void Device::setBattery(const Nullable<percent> &battery)
{
	m_battery = battery;
}

const Nullable<Device::percent> &Device::battery() const
{
	return m_battery;
}

void Device::setSignal(const Nullable<percent> &signal)
{
	m_signal = signal;
}

const Nullable<Device::percent> &Device::signal() const
{
	return m_signal;
}

void Device::setStatus(const DeviceStatus &status)
{
	m_status = status;
}

const DeviceStatus &Device::status() const
{
	return m_status;
}

DeviceStatus &Device::status()
{
	return m_status;
}

bool Device::available(const unsigned int multiple,
		const Timestamp &ref) const
{
	if (m_refresh.isNone() || m_refresh.isDisabled())
		return true;

	Timespan diff;
	for (unsigned int i = 0; i < multiple; ++i)
		diff += refresh().time();

	return m_status.lastSeen() + diff >= ref;
}
