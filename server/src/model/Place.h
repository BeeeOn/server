#ifndef BEEEON_PLACE_H
#define BEEEON_PLACE_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"

namespace BeeeOn {

/**
 * A place represents a real location (a building, home, ...).
 */
class Place : public Entity {
public:
	typedef Poco::SharedPtr<Place> Ptr;
	typedef long ID;

	Place(const std::string &data):
		Entity(data)
	{
	}
};

typedef Place::Collection PlaceCollection;
typedef Place::ID PlaceID;

}

#endif
