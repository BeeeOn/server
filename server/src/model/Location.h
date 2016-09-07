#ifndef BEEEON_LOCATION_H
#define BEEEON_LOCATION_H

#include <Poco/SharedPtr.h>
#include "model/GlobalID.h"
#include "model/Place.h"
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of a Location application entity.
 */
class Location {
public:
	typedef Poco::SharedPtr<Location> Ptr;
	typedef BeeeOn::Collection<Location> Collection;
	typedef GlobalID ID;

	Location()
	{
	}

	Location(const ID &id):
		m_id(id)
	{
	}

	Location(const Location &copy):
		m_id(copy.m_id),
		m_name(copy.m_name),
		m_place(copy.m_place)
	{
	}

	Location(const ID &id, const Location &copy):
		m_id(id),
		m_name(copy.m_name),
		m_place(copy.m_place)
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

	void setPlace(const Place &place)
	{
		m_place = place;
	}

	const Place &place() const
	{
		return m_place;
	}

private:
	ID m_id;
	std::string m_name;
	Place m_place;
};

typedef Location::Collection LocationCollection;
typedef Location::ID LocationID;

}

#endif
