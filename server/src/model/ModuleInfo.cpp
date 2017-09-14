#include "model/ModuleInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

ModuleInfo::ModuleInfo()
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

void ModuleInfo::setClassName(const string &className)
{
	m_className = className;
}

const string &ModuleInfo::className() const
{
	return m_className;
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

void ModuleInfo::setUnavailable(const string &unavailable)
{
	m_unavailable = unavailable;
}

const string &ModuleInfo::unavailable() const
{
	return m_unavailable;
}

void ModuleInfo::setDefaultValue(const string &defaultValue)
{
	m_default = defaultValue;
}

const string &ModuleInfo::defaultValue() const
{
	return m_default;
}
