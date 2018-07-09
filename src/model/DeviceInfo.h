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

		std::string name() const;
		std::string vendor() const;

	private:
		std::string m_name;
		std::string m_vendor;
	};

	class MatchGlob : public Match {
	public:
		MatchGlob(const std::string &name, const std::string &vendor);

		bool match(
			const std::string &name,
			const std::string &vendor) const override;

		std::string namePattern() const;
		std::string vendorPattern() const;

	private:
		std::string m_name;
		std::string m_vendor;
	};

	void addMatch(Match::Ptr match);
	bool match(const std::string &name, const std::string &vendor) const;

	void setName(const std::string &name);
	const std::string &name() const;

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

	const std::set<ModuleInfo> &modules() const
	{
		return m_modules;
	}

	const std::list<Match::Ptr> matchers() const
	{
		return m_match;
	}

	bool lookup(ModuleInfo &module) const;

private:
	std::string m_name;
	std::string m_vendor;
	std::list<Match::Ptr> m_match;
	std::set<ModuleInfo> m_modules;
};

typedef DeviceInfo::ID DeviceInfoID;

}

#endif
