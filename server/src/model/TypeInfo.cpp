#include <string>

#include <Poco/Exception.h>
#include <Poco/NumberFormatter.h>

#include "model/TypeInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TypeInfo::TypeInfo()
{
}

TypeInfo::TypeInfo(const TypeInfo::ID &id):
	Entity(id)
{
}

void TypeInfo::setName(const string &name)
{
	m_name = name;
}

const string &TypeInfo::name() const
{
	return m_name;
}

void TypeInfo::setUnit(const string &unit)
{
	m_unit = unit;
}

const string &TypeInfo::unit() const
{
	return m_unit;
}

std::string TypeInfo::asString(const double v) const
{
	return to_string(v);
}
