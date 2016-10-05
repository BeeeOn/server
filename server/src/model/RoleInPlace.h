#ifndef BEEEON_ROLE_IN_PLACE_H
#define BEEEON_ROLE_IN_PLACE_H

#include <Poco/SharedPtr.h>

#include "model/GlobalID.h"
#include "model/Place.h"
#include "model/Identity.h"
#include "server/AccessLevel.h"

namespace BeeeOn {

class RoleInPlace {
public:
	typedef Poco::SharedPtr<RoleInPlace> Ptr;
	typedef GlobalID ID;

	RoleInPlace();
	RoleInPlace(const ID &id);
	RoleInPlace(const RoleInPlace &copy);
	RoleInPlace(const ID &id, const RoleInPlace &copy);

	void setPlace(const Place &place)
	{
		m_place = place;
	}

	const Place &place() const
	{
		return m_place;
	}

	void setIdentity(const Identity &identity)
	{
		m_identity = identity;
	}

	const Identity &identity() const
	{
		return m_identity;
	}

	void setLevel(const AccessLevel &level)
	{
		m_level = level;
	}

	const AccessLevel &level() const
	{
		return m_level;
	}

	void setID(const ID &id)
	{
		m_id = id;
	}

	const ID &id() const
	{
		return m_id;
	}

private:
	ID m_id;
	Place m_place;
	Identity m_identity;
	AccessLevel m_level;
};

typedef RoleInPlace::ID RoleInPlaceID;

}

#endif
