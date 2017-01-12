#include <Poco/Exception.h>

#include "model/TypeInfo.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

TypeInfo::TypeInfo():
	m_cType(CTYPE_INT),
	m_width(1)
{
}

TypeInfo::TypeInfo(const TypeInfo::ID &id):
	Entity(id),
	m_cType(CTYPE_INT),
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

void TypeInfo::setCType(const string &cType)
{
	if (cType == "int")
		setCType(CTYPE_INT);
	else if (cType == "bitarray")
		setCType(CTYPE_INT);
	else if (cType == "float")
		setCType(CTYPE_FLOAT);
	else
		throw InvalidArgumentException("unrecognized C-type: " + cType);
}

void TypeInfo::setCType(const CType cType)
{
	m_cType = cType;
}

TypeInfo::CType TypeInfo::cType() const
{
	return m_cType;
}

void TypeInfo::setWidth(const unsigned int width)
{
	m_width = width;
}

unsigned int TypeInfo::width() const
{
	return m_width;
}
