#include "model/ModuleInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ModuleInfo::ModuleInfo():
	m_controllable(false)
{
}

void ModuleInfo::setType(const SharedPtr<TypeInfo> type)
{
	m_type = type;
}

const SharedPtr<TypeInfo> ModuleInfo::type() const
{
	return m_type;
}

void ModuleInfo::setSubtype(const SharedPtr<SubtypeInfo> subtype)
{
	m_subtype = subtype;
}

const SharedPtr<SubtypeInfo> ModuleInfo::subtype() const
{
	return m_subtype;
}

void ModuleInfo::setName(const string &name)
{
	m_name = name;
}

const string &ModuleInfo::name() const
{
	return m_name;
}

void ModuleInfo::setGroup(const string &group)
{
	m_group = group;
}

const string &ModuleInfo::group() const
{
	return m_group;
}

void ModuleInfo::setControllable(bool controllable)
{
	m_controllable = controllable;
}

bool ModuleInfo::isControllable() const
{
	return m_controllable;
}

bool ModuleInfo::compatible(const ModuleType &type) const
{
	return *m_type == type.type();
}

const string ModuleInfo::toString() const
{
	string buffer;

	if (type().isNull())
		buffer.append("(null)");
	else
		buffer.append(type()->name());

	if (subtype().isNull())
		buffer.append(",(null)");
	else if (subtype()->kind() == SubtypeInfo::KIND_INVALID)
		buffer.append(",none");
	else
		buffer.append(subtype()->name());

	if (isControllable())
		buffer.append(",controllable");

	return buffer;
}
