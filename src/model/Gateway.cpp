#include <Poco/Timezone.h>

#include "model/Gateway.h"

using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

Gateway::Gateway():
	m_latitude(0),
	m_longitude(0)
{
}

Gateway::Gateway(const ID &id):
	Entity<GatewayID>(id),
	m_latitude(0),
	m_longitude(0)
{
}

void Gateway::setName(const std::string &name)
{
	m_name = name;
}

const std::string &Gateway::name() const
{
	return m_name;
}

void Gateway::setAltitude(const Nullable<int> alt)
{
	m_altitude = alt;
}

void Gateway::setAltitude(int alt)
{
	m_altitude = alt;
}

Nullable<int> Gateway::altitude() const
{
	return m_altitude;
}

void Gateway::setLatitude(double lat)
{
	m_latitude = lat;
}

double Gateway::latitude() const
{
	return m_latitude;
}

void Gateway::setLongitude(double lon)
{
	m_longitude = lon;
}

double Gateway::longitude() const
{
	return m_longitude;
}

void Gateway::setLastChanged(const Nullable<DateTime> &at)
{
	m_lastChanged = at;
}

Nullable<DateTime> Gateway::lastChanged() const
{
	return m_lastChanged;
}

void Gateway::setLastActivity(const Nullable<DateTime> &at)
{
	m_lastActivity = at;
}

Nullable<DateTime> Gateway::lastActivity() const
{
	return m_lastActivity;
}

void Gateway::setVersion(const std::string &version)
{
	m_version = version;
}

std::string Gateway::version() const
{
	return m_version;
}

void Gateway::setIPAddress(const std::string &ipAddress)
{
	setIPAddress(IPAddress(ipAddress));
}

void Gateway::setIPAddress(const IPAddress &ipAddress)
{
	m_ipAddress = ipAddress;
}

IPAddress Gateway::ipAddress() const
{
	return m_ipAddress;
}

void Gateway::setTimeZone(const TimeZone &tz)
{
	m_timeZone = tz;
}

const TimeZone &Gateway::timeZone() const
{
	return m_timeZone;
}
