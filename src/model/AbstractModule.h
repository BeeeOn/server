#pragma once

#include "model/Entity.h"
#include "model/ModuleInfo.h"
#include "model/SimpleID.h"

namespace BeeeOn {

/**
 * @brief Abstract parent of a certain implementation of a device module.
 *
 * The device modules has some common features that can be shared among
 * implementations.
 *
 * @see Sensor
 */
class AbstractModule : public Entity<SimpleID> {
public:
	AbstractModule();
	AbstractModule(const ID &id);

	void setInfo(const ModuleInfo &info);
	const ModuleInfo &info() const;

	void setName(const std::string &name);

	/**
	 * @return custom name given by user.
	 */
	std::string name() const;

private:
	ModuleInfo m_info;
	std::string m_name;
};

}
