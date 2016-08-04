#ifndef BEEEON_PLACE_H
#define BEEEON_PLACE_H

#include <Poco/SharedPtr.h>
#include "model/Collection.h"

namespace BeeeOn {

/**
 * A place represents a real location (a building, home, ...).
 */
class Place {
public:
	typedef Poco::SharedPtr<Place> Ptr;
	typedef BeeeOn::Collection<Place> Collection;
	typedef long ID;

	Place()
	{
	}

	Place(const Place &copy):
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
	std::string m_name;
};

typedef Place::Collection PlaceCollection;
typedef Place::ID PlaceID;

}

#endif
