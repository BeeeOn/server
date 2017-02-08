#ifndef BEEEON_DEVICE_INFO_H
#define BEEEON_DEVICE_INFO_H

#include <string>
#include <set>

#include "model/Entity.h"
#include "model/ModuleInfo.h"
#include "model/SimpleID.h"

namespace BeeeOn {

class DeviceInfo : public Entity<SimpleID> {
public:
	DeviceInfo();
	DeviceInfo(const DeviceInfo::ID &id);

	void setName(const std::string &name);
	const std::string &name() const;

	void setDisplayName(const std::string &name);
	const std::string &displayName() const;

	void setVendor(const std::string &vendor);
	const std::string &vendor() const;

	bool add(const ModuleInfo &info);
	void clear();

	std::set<ModuleInfo>::const_iterator begin() const
	{
		return m_modules.begin();
	}

	std::set<ModuleInfo>::const_iterator end() const
	{
		return m_modules.end();
	}

	bool lookup(ModuleInfo &module) const;

private:
	std::string m_name;
	std::string m_displayName;
	std::string m_vendor;
	std::set<ModuleInfo> m_modules;
};

typedef DeviceInfo::ID DeviceInfoID;

}

#endif
