#include "model/GatewayStatus.h"

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace BeeeOn;

void GatewayStatus::setLastChanged(const Nullable<DateTime> &at)
{
	m_lastChanged = at;
}

Nullable<DateTime> GatewayStatus::lastChanged() const
{
	return m_lastChanged;
}

void GatewayStatus::setVersion(const string &version)
{
	m_version = version;
}

string GatewayStatus::version() const
{
	return m_version;
}

void GatewayStatus::setIPAddress(const string &ipAddress)
{
	setIPAddress(IPAddress(ipAddress));
}

void GatewayStatus::setIPAddress(const IPAddress &ipAddress)
{
	m_ipAddress = ipAddress;
}

IPAddress GatewayStatus::ipAddress() const
{
	return m_ipAddress;
}
