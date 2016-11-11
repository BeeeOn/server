#include "model/LegacyGateway.h"

using namespace BeeeOn;

LegacyGateway::LegacyGateway()
{
}

LegacyGateway::LegacyGateway(const GatewayID &id):
	Gateway(id)
{
}

LegacyGateway::LegacyGateway(const LegacyGateway &copy):
	Gateway(copy),
	m_owner(copy.m_owner),
	m_accessLevel(copy.m_accessLevel),
	m_deviceCount(copy.m_deviceCount),
	m_userCount(copy.m_userCount)
{
}

LegacyGateway::LegacyGateway(const GatewayID &id, const LegacyGateway &copy):
	Gateway(id, copy),
	m_owner(copy.m_owner),
	m_accessLevel(copy.m_accessLevel),
	m_deviceCount(copy.m_deviceCount),
	m_userCount(copy.m_userCount)
{
}

LegacyGateway::LegacyGateway(const Gateway &copy):
	Gateway(copy)
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

