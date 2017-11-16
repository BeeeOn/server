#ifndef BEEEON_LOCATION_H
#define BEEEON_LOCATION_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"
#include "model/GlobalID.h"
#include "model/Gateway.h"

namespace BeeeOn {

/**
 * Representation of a Location application entity.
 */
class Location : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<Location> Ptr;

	Location();
	Location(const ID &id);

	void setName(const std::string &name);
	const std::string &name() const;

	void setGateway(const Gateway &gateway);
	const Gateway &gateway() const;

private:
	std::string m_name;
	Gateway m_gateway;
};

typedef Location::ID LocationID;

}

#endif
