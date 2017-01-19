#ifndef BEEEON_PLACE_H
#define BEEEON_PLACE_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * A place represents a real location (a building, home, ...).
 */
class Place : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<Place> Ptr;

	Place()
	{
	}

	Place(const ID &id):
		Entity(id)
	{
	}

	Place(const Place &copy):
		Entity(copy),
		m_name(copy.m_name)
	{
	}

	Place(const ID &id, const Place &copy):
		Entity(id, copy),
		m_name(copy.m_name)
	{
	}

	void setName(const std::string &name)
	{
		m_name = name;
	}

	const std::string name() const
	{
		return m_name;
	}

private:
	std::string m_name;
};

typedef Place::ID PlaceID;

}

#endif
