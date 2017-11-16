#include <Poco/Dynamic/Var.h>

#include "work/WorkContent.h"

using namespace std;
using namespace Poco;
using namespace Poco::JSON;
using namespace BeeeOn;

WorkContent::WorkContent():
	m_json(new Object)
{
}

WorkContent::WorkContent(const WorkContent &copy):
	m_type(copy.m_type),
	m_json(new Object(*copy.m_json))
{
}

WorkContent::WorkContent(const ClassInfo &type):
	m_type(type),
	m_json(new Object)
{
	m_json->set("type", type.name());
}

WorkContent::WorkContent(const Object::Ptr json):
	m_type(ClassInfo::byName(json->get("type"))),
	m_json(json)
{
}

void WorkContent::setType(const ClassInfo &type)
{
	m_type = type;
	m_json->set("type", type.name());
}

ClassInfo WorkContent::type() const
{
	return m_type;
}

string WorkContent::toString() const
{
	return Dynamic::Var::toString(m_json);
}

Object::Ptr WorkContent::json() const
{
	return m_json;
}
