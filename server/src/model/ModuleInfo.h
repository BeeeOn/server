#ifndef BEEEON_MODULE_INFO_H
#define BEEEON_MODULE_INFO_H

#include <string>

#include <Poco/SharedPtr.h>

#include "model/Entity.h"
#include "model/SimpleID.h"
#include "model/TypeInfo.h"

namespace BeeeOn {

class ModuleInfo : public Entity<SimpleID> {
public:
	ModuleInfo();

	void setType(const Poco::SharedPtr<TypeInfo> type);
	const Poco::SharedPtr<TypeInfo> type() const;

	void setClassName(const std::string &type);
	const std::string &className() const;

	void setName(const std::string &group);
	const std::string &name() const;

	void setGroup(const std::string &group);
	const std::string &group() const;

	void setUnavailable(const std::string &unavailable);
	const std::string &unavailable() const;

	void setDefaultValue(const std::string &defaultValue);
	const std::string &defaultValue() const;

	void setControllable(bool controllable);
	bool isControllable() const;

private:
	Poco::SharedPtr<TypeInfo> m_type;
	std::string m_className;
	std::string m_name;
	std::string m_group;
	std::string m_unavailable;
	std::string m_default;
	bool m_controllable;
};

typedef ModuleInfo::ID ModuleInfoID;

}

#endif
