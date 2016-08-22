#include "JSONSerializer.h"

namespace BeeeOn {

JSONObjectSerializer::JSONObjectSerializer():
	m_object(new Poco::JSON::Object())
{
}

JSONObjectSerializer::JSONObjectSerializer(const std::string &s):
	m_object(Poco::JSON::Parser().parse(s)
		.extract<Poco::JSON::Object::Ptr>())
{
}

JSONObjectSerializer::JSONObjectSerializer(Poco::JSON::Object::Ptr object):
	m_object(object)
{
}

void JSONObjectSerializer::clear()
{
	m_object->clear();
}

void JSONObjectSerializer::clearAndSetup(const Poco::JSON::Object::Ptr &o)
{
	m_object = o;
}

std::string JSONObjectSerializer::toString() const
{
	std::ostringstream s;
	m_object->stringify(s);
	return s.str();
}

const Poco::JSON::Object::Ptr JSONObjectSerializer::obtain()
{
	return m_object;
}


JSONArraySerializer::JSONArraySerializer():
	m_array(new Poco::JSON::Array())
{
}

JSONArraySerializer::JSONArraySerializer(const std::string &s):
	m_array(Poco::JSON::Parser().parse(s)
		.extract<Poco::JSON::Array::Ptr>())
{
}

JSONArraySerializer::JSONArraySerializer(Poco::JSON::Array::Ptr array):
	m_array(array)
{
}

JSONObjectSerializer JSONArraySerializer::startObject()
{
	return JSONObjectSerializer();
}

void JSONArraySerializer::endObject(JSONObjectSerializer &s)
{
	Poco::Dynamic::Var object(s.obtain());
	m_array->set(m_array->size(), object);
}

template <typename T>
void JSONArraySerializer::push(const T &v)
{
	Poco::Dynamic::Var value(v);
	m_array->set(m_array->size(), value);
}

template <typename T>
T JSONArraySerializer::get(const unsigned int i) const
{
	return m_array->getElement<T>(i);
}

JSONObjectSerializer JSONArraySerializer::getObject(unsigned int i)
{
	return JSONObjectSerializer(m_array->getObject(i));
}

bool JSONArraySerializer::isObject(const unsigned int i) const
{
	return m_array->isObject(i);
}

unsigned int JSONArraySerializer::size() const
{
	return m_array->size();
}

std::string JSONArraySerializer::toString() const
{
	std::ostringstream s;
	m_array->stringify(s);
	return s.str();
}

void JSONArraySerializer::clear()
{
	m_array->clear();
}

void JSONArraySerializer::clearAndSetup(Poco::JSON::Array::Ptr &a)
{
	m_array = a;
}

Poco::JSON::Array::Ptr JSONArraySerializer::obtain()
{
	return m_array;
}

}
