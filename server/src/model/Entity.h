#ifndef BEEEON_ENTITY_H
#define BEEEON_ENTITY_H

#include <ostream>
#include <Poco/SharedPtr.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Array.h>

namespace BeeeOn {

class EntityCollection;

/**
 * Entity is a common ancestor of all data objects in the system.
 * It is not an application interface. Its purpose is to accomodate
 * objects to work on all layers.
 *
 * The Entity is constructed from a JSON string or a parsed JSON
 * object. It is also serializable back to the JSON format.
 * This is just a property of the current design (not an API) and
 * can be changed in the future to another shinny bright protocol.
 */
class Entity {
public:
	typedef Poco::SharedPtr<Entity> Ptr;
	typedef EntityCollection Collection;

	Entity() {}
	Entity(const std::string &data);
	Entity(const Poco::Dynamic::Var &data)
	{
		constructor(data);
	}

	Entity(Poco::JSON::Object::Ptr data):
		m_data(data)
	{
	}

	Entity(const Entity &e):
		m_data(e.m_data)
	{
	}

	friend std::ostream &operator <<(std::ostream &o, Entity const &e)
	{
		e.print(o);
		return o;
	}

	/**
	 * Convert self to string. The current output format is JSON.
	 */
	const std::string toString() const;

protected:
	/**
	 * Print self to an output stream. The current output format
	 * is JSON.
	 */
	virtual void print(std::ostream &o) const;

	/**
	 * Extract a value from the serialized data with
	 * a type-safe conversion.
	 */
	template <typename T>
	const T get(const std::string &key) const
	{
		return m_data->getValue<T>(key);
	}

	/**
	 * Extract a value from the serialized data.
	 */
	const std::string get(const std::string &key) const
	{
		return get<std::string>(key);
	}

	/**
	 * Common constructor code, called from constructors only.
	 */
	void constructor(const Poco::Dynamic::Var &data);

private:
	Poco::JSON::Object::Ptr m_data;
};

/**
 * EntityCollection represents a collection of Entity instances.
 * It is a heterogenous collection (so avoid any polymorphism at
 * the Entity level).
 *
 * The collection is constructed from JSON data and can be serialied
 * back to the JSON string format. This is a property of the current
 * design (not an API).
 */
class EntityCollection {
public:
	EntityCollection() {}
	EntityCollection(const std::string &data);
	EntityCollection(const Poco::Dynamic::Var &data)
	{
		constructor(data);
	}

	EntityCollection(Poco::JSON::Array::Ptr data):
		m_data(data)
	{
	}

	EntityCollection(const EntityCollection &e):
		m_data(e.m_data)
	{
	}

	friend std::ostream &operator <<(std::ostream &o,
			EntityCollection const &c)
	{
		c.print(o);
		return o;
	}

	/**
	 * Convert self to string. The current output format is JSON.
	 */
	const std::string toString() const;

	std::size_t size() const
	{
		return m_data->size();
	}

protected:
	/**
	 * Print self to an output stream. The current out format
	 * is JSON.
	 */
	virtual void print(std::ostream &o) const;

	template <typename T>
	const T get(const unsigned int index) const
	{
		return m_data->getElement<T>(index);
	}

	/**
	 * Common constructor code, called from constructors only.
	 */
	void constructor(const Poco::Dynamic::Var &data);

private:
	Poco::JSON::Array::Ptr m_data;
};

};

#endif
