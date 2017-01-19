#include "model/RoleInPlace.h"

using namespace BeeeOn;

RoleInPlace::RoleInPlace()
{
}

RoleInPlace::RoleInPlace(const ID &id):
	Entity(id)
{
}

RoleInPlace::RoleInPlace(const RoleInPlace &copy):
	Entity(copy),
	m_place(copy.m_place),
	m_identity(copy.m_identity),
	m_level(copy.m_level)
{
}

RoleInPlace::RoleInPlace(const ID &id, const RoleInPlace &copy):
	Entity(id, copy),
	m_place(copy.m_place),
	m_identity(copy.m_identity),
	m_level(copy.m_level)
{
}
