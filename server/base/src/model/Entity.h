#ifndef BEEEON_ENTITY_H
#define BEEEON_ENTITY_H

#include <string>

namespace BeeeOn {

class EntityLoader;

/**
 * Common ancestor of model classes.
 */
template <typename TypeID>
class Entity {
	friend EntityLoader;
public:
	typedef TypeID ID;

	Entity();
	Entity(const TypeID &id);

	virtual ~Entity()
	{
	}

	void setId(const TypeID &id);
	const TypeID &id() const;
	bool hasId() const;

	bool loaded() const;

	virtual const std::string toString() const;

	bool operator <(const Entity<TypeID> &e) const
	{
		return m_id < e.m_id;
	}

protected:
	void setLoaded(bool loaded);

private:
	TypeID m_id;
	bool m_loaded;
};

template <typename TypeID>
inline std::string operator +(const char *s, const Entity<TypeID> &e)
{
	return std::string(s) + e.toString();
}

template <typename TypeID>
inline std::string operator +(const std::string &s, const Entity<TypeID> &e)
{
	return s + e.toString();
}

template <typename TypeID>
Entity<TypeID>::Entity():
	m_loaded(false)
{
}

template <typename TypeID>
Entity<TypeID>::Entity(const TypeID &id):
	m_id(id),
	m_loaded(false)
{
}

template <typename TypeID>
void Entity<TypeID>::setId(const TypeID &id)
{
	m_id = id;
}

template <typename TypeID>
const TypeID &Entity<TypeID>::id() const
{
	return m_id;
}

template <typename TypeID>
bool Entity<TypeID>::hasId() const
{
	return !m_id.isNull();
}

template <typename TypeID>
void Entity<TypeID>::setLoaded(bool loaded)
{
	m_loaded = loaded;
}

template <typename TypeID>
bool Entity<TypeID>::loaded() const
{
	return m_loaded;
}

template <typename TypeID>
const std::string Entity<TypeID>::toString() const
{
	return m_id.toString();
}

}

#endif
