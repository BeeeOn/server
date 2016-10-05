#include "model/RoleInPlace.h"

using namespace BeeeOn;

RoleInPlace::RoleInPlace()
{
}

RoleInPlace::RoleInPlace(const ID &id):
	m_id(id)
{
}

RoleInPlace::RoleInPlace(const RoleInPlace &copy):
	m_id(copy.m_id),
	m_place(copy.m_place),
	m_identity(copy.m_identity),
	m_level(copy.m_level)
{
}

RoleInPlace::RoleInPlace(const ID &id, const RoleInPlace &copy):
	m_id(id),
	m_place(copy.m_place),
	m_identity(copy.m_identity),
	m_level(copy.m_level)
{
}
