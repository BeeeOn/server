#include "model/LegacyGateway.h"

using namespace BeeeOn;

LegacyGateway::LegacyGateway()
{
}

LegacyGateway::LegacyGateway(const GatewayID &id):
	Gateway(id)
{
}

void LegacyGateway::setOwner(const User &owner)
{
	m_owner = owner;
}

const User &LegacyGateway::owner() const
{
	return m_owner;
}

void LegacyGateway::setAccessLevel(const AccessLevel &level)
{
	m_accessLevel = level;
}

const AccessLevel &LegacyGateway::accessLevel() const
{
	return m_accessLevel;
}

void LegacyGateway::setDeviceCount(unsigned int count)
{
	m_deviceCount = count;
}

unsigned int LegacyGateway::deviceCount() const
{
	return m_deviceCount;
}

void LegacyGateway::setUserCount(unsigned int count)
{
	m_userCount = count;
}

unsigned int LegacyGateway::userCount() const
{
	return m_userCount;
}

