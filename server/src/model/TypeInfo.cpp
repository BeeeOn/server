#include <string>

#include <Poco/Exception.h>
#include <Poco/NumberFormatter.h>

#include "model/TypeInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TypeInfo::TypeInfo():
	m_width(1)
{
}

TypeInfo::TypeInfo(const TypeInfo::ID &id):
	Entity(id),
	m_width(1)
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

void TypeInfo::setWidth(const unsigned int width)
{
	m_width = width;
}

unsigned int TypeInfo::width() const
{
	return m_width;
}

std::string TypeInfo::asString(const double v) const
{
	return to_string(v);
}
