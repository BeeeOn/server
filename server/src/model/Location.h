#ifndef BEEEON_LOCATION_H
#define BEEEON_LOCATION_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"

namespace BeeeOn {

/**
 * Representation of a Location application entity.
 */
class Location : public Entity {
public:
	typedef Poco::SharedPtr<Location> Ptr;
	typedef long ID;

	Location(const std::string &data):
		Entity(data)
	{
	}

	Location(const Location &copy):
		Entity(copy)
	{
	}
};

typedef Location::Collection LocationCollection;
typedef Location::ID LocationID;

}

#endif
