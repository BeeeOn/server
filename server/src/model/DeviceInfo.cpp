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

void DeviceInfo::addMatch(Match::Ptr match)
{
	m_match.push_back(match);
}

bool DeviceInfo::match(const string &name, const string &vendor) const
{
	for (const auto m : m_match) {
		if (m->match(name, vendor))
			return true;
	}

	return false;
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

void DeviceInfo::setDisplayVendor(const string &vendor)
{
	m_displayVendor = vendor;
}

const string &DeviceInfo::displayVendor() const
{
	return m_displayVendor;
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

bool DeviceInfo::lookup(ModuleInfo &module) const
{
	auto it = m_modules.find(module);
	if (it == m_modules.end())
		return false;

	module = *it;
	return true;
}

DeviceInfo::Match::~Match()
{
}

DeviceInfo::MatchExact::MatchExact(
		const string &name, const string &vendor):
	m_name(name),
	m_vendor(vendor)
{
}

bool DeviceInfo::MatchExact::match(
		const string &name, const string &vendor) const
{
	return m_name == name && m_vendor == vendor;
}
