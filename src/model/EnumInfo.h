#ifndef BEEEON_ENUM_INFO_H
#define BEEEON_ENUM_INFO_H

#include <map>
#include <set>
#include <string>

#include "model/TypeInfo.h"

namespace BeeeOn {

class EnumInfo {
public:
	typedef TypeInfo::Level Level;

	EnumInfo();

	void setValues(const std::map<int, std::string> &values);
	const std::map<int, std::string> &values() const;

	void setLevels(const std::set<Level> &levels);
	const std::set<Level> &levels() const;

	bool has(const int &value) const;
	const std::string &get(const int &value) const;

private:
	std::map<int, std::string> m_values;
	std::set<Level> m_levels;
};

}

#endif
