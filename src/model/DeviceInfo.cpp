#include <Poco/Glob.h>

#include "model/DeviceInfo.h"

using namespace std;
using namespace Poco;
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

bool DeviceInfo::lookup(ModuleInfo &module) const
{
	auto it = m_modules.find(module);
	if (it == m_modules.end())
		return false;

	module = *it;
	return true;
}

const string DeviceInfo::toString() const
{
	return m_vendor + " " + m_name + " (" + to_string(m_modules.size()) + ")";
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

string DeviceInfo::MatchExact::name() const
{
	return m_name;
}

string DeviceInfo::MatchExact::vendor() const
{
	return m_vendor;
}

DeviceInfo::MatchGlob::MatchGlob(
		const string &name, const string &vendor):
	m_name(name),
	m_vendor(vendor)
{
}

bool DeviceInfo::MatchGlob::match(
		const string &name, const string &vendor) const
{
	Glob namePat(m_name);
	Glob vendorPat(m_vendor);

	return namePat.match(name) && vendorPat.match(vendor);
}

string DeviceInfo::MatchGlob::namePattern() const
{
	return m_name;
}

string DeviceInfo::MatchGlob::vendorPattern() const
{
	return m_vendor;
}
