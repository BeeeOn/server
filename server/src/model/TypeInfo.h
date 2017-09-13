#ifndef BEEEON_TYPE_INFO_H
#define BEEEON_TYPE_INFO_H

#include <string>

#include "model/Entity.h"
#include "model/SimpleID.h"

namespace BeeeOn {

class TypeInfo : public Entity<SimpleID> {
public:
	TypeInfo();
	TypeInfo(const TypeInfo::ID &id);

	void setName(const std::string &name);
	const std::string &name() const;

	void setUnit(const std::string &unit);
	const std::string &unit() const;

	void setWidth(const unsigned int width);
	unsigned int width() const;

	std::string asString(const double v) const;

private:
	std::string m_name;
	std::string m_unit;
	unsigned int m_width;
};

typedef TypeInfo::ID TypeInfoID;

}

#endif
