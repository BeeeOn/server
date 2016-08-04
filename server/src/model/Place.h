#ifndef BEEEON_PLACE_H
#define BEEEON_PLACE_H

#include <Poco/SharedPtr.h>
#include "model/SimpleID.h"
#include "model/Collection.h"

namespace BeeeOn {

/**
 * A place represents a real location (a building, home, ...).
 */
class Place {
public:
	typedef Poco::SharedPtr<Place> Ptr;
	typedef BeeeOn::Collection<Place> Collection;
	typedef SimpleID ID;

	Place()
	{
	}

	Place(ID id):
		m_id(id)
	{
	}

	Place(const Place &copy):
		m_id(copy.m_id),
		m_name(copy.m_name)
	{
	}

	Place(ID id, const Place &copy):
		m_id(id),
		m_name(copy.m_name)
	{
	}

	ID id() const
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

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void toWeb(Serializer &s) const
	{
		s.push("name", name());
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
	ID m_id = 0;
	std::string m_name;
};

typedef Place::Collection PlaceCollection;
typedef Place::ID PlaceID;

}

#endif
