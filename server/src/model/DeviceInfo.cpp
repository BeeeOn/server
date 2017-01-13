#include "model/DeviceInfo.h"

using namespace std;
using namespace BeeeOn;

DeviceInfo::DeviceInfo()
{
}

DeviceInfo::DeviceInfo(const DeviceInfo::ID &id):
	Entity(id)
{
}

void DeviceInfo::setName(const string &name)
{
	m_name = name;
}

const string &DeviceInfo::name() const
{
	return m_name;
}

void DeviceInfo::setDisplayName(const string &name)
{
	m_displayName = name;
}

const string &DeviceInfo::displayName() const
{
	return m_displayName;
}

void DeviceInfo::setVendor(const string &vendor)
{
	m_vendor = vendor;
}

const string &DeviceInfo::vendor() const
{
	return m_vendor;
}

bool DeviceInfo::add(const ModuleInfo &info)
{
	if (m_modules.find(info) != m_modules.end())
		return false;

	m_modules.insert(info);
	return true;
}

void DeviceInfo::clear()
{
	m_modules.clear();
}
