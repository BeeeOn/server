#include <Poco/Timespan.h>

#include "model/Device.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

Device::Device()
{
}

Device::Device(const ID &id):
	m_id(id)
{
}

Device::Device(const Device &copy):
	m_id(copy.m_id),
	m_gateway(copy.m_gateway),
	m_location(copy.m_location),
	m_name(copy.m_name),
	m_type(copy.m_type),
	m_refresh(copy.m_refresh),
	m_battery(copy.m_battery),
	m_signal(copy.m_signal),
	m_firstSeen(copy.m_firstSeen),
	m_lastSeen(copy.m_lastSeen),
	m_activeSince(copy.m_activeSince)

{
}

Device::Device(const ID &id, const Device &copy):
	m_id(id),
	m_gateway(copy.m_gateway),
	m_location(copy.m_location),
	m_name(copy.m_name),
	m_type(copy.m_type),
	m_refresh(copy.m_refresh),
	m_battery(copy.m_battery),
	m_signal(copy.m_signal),
	m_firstSeen(copy.m_firstSeen),
	m_lastSeen(copy.m_lastSeen),
	m_activeSince(copy.m_activeSince)
{
}

const Device::ID &Device::id() const
{
	return m_id;
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

void Device::setType(const unsigned int type)
{
	m_type = type;
}

unsigned int Device::type() const
{
	return m_type;
}

void Device::setRefresh(const Timespan &refresh)
{
	m_refresh = refresh;
}

const Timespan &Device::refresh() const
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

void Device::setFirstSeen(const DateTime &at)
{
	m_firstSeen = at;
}

const DateTime &Device::firstSeen() const
{
	return m_firstSeen;
}

void Device::setLastSeen(const DateTime &at)
{
	m_lastSeen = at;
}

const DateTime &Device::lastSeen() const
{
	return m_lastSeen;
}

bool Device::available(const unsigned int multiple,
		const DateTime &ref) const
{
	Timespan diff;
	for (unsigned int i = 0; i < multiple; ++i)
		diff += refresh();

	return lastSeen() + diff >= ref;
}

void Device::setActiveSince(const Nullable<DateTime> &at)
{
	m_activeSince = at;
}

const Nullable<DateTime> &Device::activeSince() const
{
	return m_activeSince;
}
