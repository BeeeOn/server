#ifndef BEEEON_DEVICE_INFO_H
#define BEEEON_DEVICE_INFO_H

#include <list>
#include <string>
#include <set>

#include <Poco/SharedPtr.h>

#include "model/Entity.h"
#include "model/ModuleInfo.h"
#include "model/SimpleID.h"

namespace BeeeOn {

class DeviceInfo : public Entity<SimpleID> {
public:
	DeviceInfo();
	DeviceInfo(const DeviceInfo::ID &id);

	class Match {
	public:
		typedef Poco::SharedPtr<Match> Ptr;

		virtual ~Match();

		virtual bool match(
			const std::string &name,
			const std::string &vendor) const = 0;
	};

	class MatchExact : public Match {
	public:
		MatchExact(const std::string &name, const std::string &vendor);

		bool match(
			const std::string &name,
			const std::string &vendor) const override;

	private:
		std::string m_name;
		std::string m_vendor;
	};

	void addMatch(Match::Ptr match);
	bool match(const std::string &name, const std::string &vendor) const;

	void setName(const std::string &name);
	const std::string &name() const;

	void setDisplayName(const std::string &name);
	const std::string &displayName() const;

	void setVendor(const std::string &vendor);
	const std::string &vendor() const;

	void setDisplayVendor(const std::string &vendor);
	const std::string &displayVendor() const;

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
	std::string m_displayVendor;
	std::list<Match::Ptr> m_match;
	std::set<ModuleInfo> m_modules;
};

typedef DeviceInfo::ID DeviceInfoID;

}

#endif
