#ifndef BEEEON_PLACE_H
#define BEEEON_PLACE_H

#include <Poco/SharedPtr.h>
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * A place represents a real location (a building, home, ...).
 */
class Place {
public:
	typedef Poco::SharedPtr<Place> Ptr;
	typedef GlobalID ID;

	Place()
	{
	}

	Place(const ID &id):
		m_id(id)
	{
	}

	Place(const Place &copy):
		m_id(copy.m_id),
		m_name(copy.m_name)
	{
	}

	Place(const ID &id, const Place &copy):
		m_id(id),
		m_name(copy.m_name)
	{
	}

	const ID &id() const
	{
		return m_id;
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
	ID m_id;
	std::string m_name;
};

typedef Place::ID PlaceID;

}

#endif
