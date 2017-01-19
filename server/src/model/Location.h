#ifndef BEEEON_LOCATION_H
#define BEEEON_LOCATION_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"
#include "model/GlobalID.h"
#include "model/Place.h"

namespace BeeeOn {

/**
 * Representation of a Location application entity.
 */
class Location : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<Location> Ptr;

	Location()
	{
	}

	Location(const ID &id):
		Entity(id)
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

	void setPlace(const Place &place)
	{
		m_place = place;
	}

	const Place &place() const
	{
		return m_place;
	}

	bool hasPlace() const
	{
		return !m_place.id().isNull();
	}

private:
	std::string m_name;
	Place m_place;
};

typedef Location::ID LocationID;

}

#endif
