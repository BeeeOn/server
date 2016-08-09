#ifndef BEEEON_JSON_SERIALIZER_H
#define BEEEON_JSON_SERIALIZER_H

#include <ostream>
#include <sstream>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>

namespace BeeeOn {

/**
 * Serialize or deserialize any object using the JSON format.
 */
class JSONObjectSerializer {
public:
	/**
	 * Construct a serializer.
	 */
	JSONObjectSerializer():
		m_object(new Poco::JSON::Object())
	{
	}

	/**
	 * Construct a deserializer from string.
	 */
	JSONObjectSerializer(const std::string &s):
		m_object(Poco::JSON::Parser().parse(s)
			.extract<Poco::JSON::Object::Ptr>())
	{
	}

	/**
	 * Construct a deserializer.
	 */
	JSONObjectSerializer(Poco::JSON::Object::Ptr object):
		m_object(object)
	{
	}

	/**
	 * Serialize an instance attribute of type T by name.
	 */
	template <typename T>
	void push(const std::string &name, const T &v)
	{
		Poco::Dynamic::Var value(v);
		m_object->set(name, value);
	}

	/**
	 * Deserialize an instance attribute of type T by name.
	 * Returns false if such attribute does not exist.
	 */
	template <typename T>
	bool get(const std::string &name, T &t) const
	{
		if (!m_object->has(name))
			return false;

		t = m_object->getValue<T>(name);
		return true;
	}

	/**
	 * Clear the serializer for another serialization.
	 *
	 * This is not a part of the serialization API.
	 */
	void clear()
	{
		m_object->clear();
	}

	/**
	 * Clear the deserializer for another deserialization.
	 * Feed it with new data.
	 *
	 * This is not a part of the serialization API.
	 */
	void clearAndSetup(const Poco::JSON::Object::Ptr &o)
	{
		m_object = o;
	}

	/**
	 * Convert serialized object into string.
	 */
	std::string toString() const
	{
		std::ostringstream s;
		m_object->stringify(s);
		return s.str();
	}

	/**
	 * Obtain the JSON Object used for serialization
	 * and deserialization.
	 *
	 * This is not a part of the serialization API.
	 */
	const Poco::JSON::Object::Ptr obtain()
	{
		return m_object;
	}

private:
	Poco::JSON::Object::Ptr m_object;
};

/**
 * Serialize or deserialize any array of data.
 */
class JSONArraySerializer {
public:
	/**
	 * Construct a serializer.
	 */
	JSONArraySerializer():
		m_array(new Poco::JSON::Array())
	{
	}

	/**
	 * Construct a deserializer from string.
	 */
	JSONArraySerializer(const std::string &s):
		m_array(Poco::JSON::Parser().parse(s)
			.extract<Poco::JSON::Array::Ptr>())
	{
	}

	/**
	 * Construct a deserializer from JSON Array.
	 */
	JSONArraySerializer(Poco::JSON::Array::Ptr array):
		m_array(array)
	{
	}

	/**
	 * Start serialization of a member of type Object.
	 * Returns a serializer for this purpose.
	 */
	JSONObjectSerializer startObject()
	{
		return JSONObjectSerializer();
	}

	/**
	 * Finish serialization of an Object member.
	 */
	void endObject(JSONObjectSerializer &s)
	{
		Poco::Dynamic::Var object(s.obtain());
		m_array->set(m_array->size(), object);
	}

	/**
	 * Serialize a value of the type T. It is appended to the
	 * end of the Array.
	 */
	template <typename T>
	void push(const T &v)
	{
		Poco::Dynamic::Var value(v);
		m_array->set(m_array->size(), value);
	}

	/**
	 * Deserialize a value of type T at index i.
	 */
	template <typename T>
	T get(const unsigned int i) const
	{
		return m_array->getElement<T>(i);
	}

	/**
	 * Provides a deserializer for the object at index i.
	 */
	JSONObjectSerializer getObject(unsigned int i)
	{
		return JSONObjectSerializer(m_array->getObject(i));
	}

	/**
	 * Test whether the element at index i is of type Object.
	 */
	bool isObject(const unsigned int i) const
	{
		return m_array->isObject(i);
	}

	/**
	 * Return the current size of the array being serialized
	 * or deserialized. The size increases during serialization.
	 * During deserialization it provides information about
	 * the amount of elements to be extracted.
	 */
	unsigned int size() const
	{
		return m_array->size();
	}

	/**
	 * Convert the serialized array into string.
	 */
	std::string toString() const
	{
		std::ostringstream s;
		m_array->stringify(s);
		return s.str();
	}

	/**
	 * Clear the serializer for another serialization.
	 *
	 * This is not a part of the serialization API.
	 */
	void clear()
	{
		m_array->clear();
	}

	/**
	 * Clear the deserializer for another deserialization.
	 *
	 * This is not a part of the serialization API.
	 */
	void clearAndSetup(Poco::JSON::Array::Ptr &a)
	{
		m_array = a;
	}

	/**
	 * Obtain the JSON Array used for serialization
	 * and deserialization.
	 *
	 * This is not a part of the serialization API.
	 */
	Poco::JSON::Array::Ptr obtain()
	{
		return m_array;
	}

private:
	Poco::JSON::Array::Ptr m_array;
};

}

#endif