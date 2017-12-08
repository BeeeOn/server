#include "model/DeviceStatus.h"

using namespace Poco;
using namespace BeeeOn;

DeviceStatus::DeviceStatus():
	m_state(STATE_INACTIVE)
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

void DeviceStatus::setLastChanged(const Timestamp &at)
{
	m_lastChanged = at;
}

const Timestamp &DeviceStatus::lastChanged() const
{
	return m_lastChanged;
}

void DeviceStatus::setState(State state)
{
	m_state = state;
}

DeviceStatus::State DeviceStatus::state() const
{
	return m_state;
}
