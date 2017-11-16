#include "model/LegacyRoleInGateway.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

LegacyRoleInGateway::LegacyRoleInGateway():
	m_owner(false)
{
}

LegacyRoleInGateway::LegacyRoleInGateway(const RoleInGatewayID &id):
	RoleInGateway(id),
	m_owner(false)
{
}

void LegacyRoleInGateway::setOwner(bool owner)
{
	m_owner = owner;
}

bool LegacyRoleInGateway::isOwner() const
{
	return m_owner;
}

void LegacyRoleInGateway::setFirstName(const string &name)
{
	m_firstName = name;
}

const string &LegacyRoleInGateway::firstName() const
{
	return m_firstName;
}

void LegacyRoleInGateway::setLastName(const string &name)
{
	m_lastName = name;
}

const string &LegacyRoleInGateway::lastName() const
{
	return m_lastName;
}

void LegacyRoleInGateway::setPicture(const URI &picture)
{
	m_picture = picture;
}

const URI &LegacyRoleInGateway::picture() const
{
	return m_picture;
}
