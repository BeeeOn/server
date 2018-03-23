#include <Poco/Exception.h>

#include "model/EnumInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

EnumInfo::EnumInfo()
{
}

void EnumInfo::setValues(const map<int, string> &values)
{
	m_values = values;
}

const map<int, string> &EnumInfo::values() const
{
	return m_values;
}

void EnumInfo::setLevels(const set<Level> &levels)
{
	m_levels = levels;
}

const set<EnumInfo::Level> &EnumInfo::levels() const
{
	return m_levels;
}

bool EnumInfo::has(const int &value) const
{
	return m_values.find(value) == m_values.end();
}

const string &EnumInfo::get(const int &value) const
{
	auto it = m_values.find(value);
	if (it == m_values.end())
		throw NotFoundException("value " + to_string(value) + " not found");

	return it->second;
}
