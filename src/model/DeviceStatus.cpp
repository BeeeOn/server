#include "model/DeviceStatus.h"

using namespace Poco;
using namespace BeeeOn;

DeviceStatus::DeviceStatus()
{
}

DeviceStatus::~DeviceStatus()
{
}

void DeviceStatus::setFirstSeen(const Timestamp &at)
{
	m_firstSeen = at;
}

const Timestamp &DeviceStatus::firstSeen() const
{
	return m_firstSeen;
}

void DeviceStatus::setLastSeen(const Timestamp &at)
{
	m_lastSeen = at;
}

const Timestamp &DeviceStatus::lastSeen() const
{
	return m_lastSeen;
}

void DeviceStatus::setActiveSince(const Nullable<Timestamp> &at)
{
	m_activeSince = at;
}

const Nullable<Timestamp> &DeviceStatus::activeSince() const
{
	return m_activeSince;
}
