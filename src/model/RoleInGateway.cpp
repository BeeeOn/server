#include <Poco/DateTime.h>

#include "model/RoleInGateway.h"

using namespace Poco;
using namespace BeeeOn;

RoleInGateway::RoleInGateway()
{
}

RoleInGateway::RoleInGateway(const ID &id):
	Entity(id)
{
}

void RoleInGateway::setGateway(const Gateway &gateway)
{
	m_gateway = gateway;
}

const Gateway &RoleInGateway::gateway() const
{
	return m_gateway;
}

void RoleInGateway::setIdentity(const Identity &identity)
{
	m_identity = identity;
}

const Identity &RoleInGateway::identity() const
{
	return m_identity;
}

void RoleInGateway::setLevel(const AccessLevel &level)
{
	m_level = level;
}

const AccessLevel &RoleInGateway::level() const
{
	return m_level;
}

void RoleInGateway::setCreated(const DateTime &created)
{
	m_created = created;
}

const Poco::DateTime &RoleInGateway::created() const
{
	return m_created;
}
