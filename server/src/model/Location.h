#ifndef BEEEON_LOCATION_H
#define BEEEON_LOCATION_H

#include <Poco/SharedPtr.h>
#include "model/SimpleID.h"
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of a Location application entity.
 */
class Location {
public:
	typedef Poco::SharedPtr<Location> Ptr;
	typedef BeeeOn::Collection<Location> Collection;
	typedef SimpleID ID;

	Location()
	{
	}

	Location(const ID &id):
		m_id(id)
	{
	}

	Location(const Location &copy):
		m_name(copy.m_name)
	{
	}

	Location(const ID &id, const Location &copy):
		m_id(id),
		m_name(copy.m_name)
	{
	}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string &name() const
	{
		return m_name;
	}

	const ID &id() const
	{
		return m_id;
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void toWeb(Serializer &s) const
	{
		s.push("name", m_name);
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void fromWeb(Serializer &s)
	{
		s.get("name", m_name);
	}

private:
	ID m_id;
	std::string m_name;
};

typedef Location::Collection LocationCollection;
typedef Location::ID LocationID;

}

#endif
