#ifndef BEEEON_MODULE_INFO_H
#define BEEEON_MODULE_INFO_H

#include <string>

#include <Poco/SharedPtr.h>

#include "model/Entity.h"
#include "model/SimpleID.h"
#include "model/SubtypeInfo.h"
#include "model/TypeInfo.h"

namespace BeeeOn {

class ModuleInfo : public Entity<SimpleID> {
public:
	ModuleInfo();

	void setType(const Poco::SharedPtr<TypeInfo> type);
	const Poco::SharedPtr<TypeInfo> type() const;

	void setSubtype(const Poco::SharedPtr<SubtypeInfo> e);
	const Poco::SharedPtr<SubtypeInfo> subtype() const;

	void setName(const std::string &group);
	const std::string &name() const;

	void setGroup(const std::string &group);
	const std::string &group() const;

	void setUnavailable(const std::string &unavailable);
	const std::string &unavailable() const;

	void setControllable(bool controllable);
	bool isControllable() const;

	/**
	 * @return summary of the ModuleInfo internals
	 */
	const std::string toString() const override;

private:
	Poco::SharedPtr<TypeInfo> m_type;
	Poco::SharedPtr<SubtypeInfo> m_subtype;
	std::string m_name;
	std::string m_group;
	std::string m_unavailable;
	bool m_controllable;
};

typedef ModuleInfo::ID ModuleInfoID;

}

#endif
