#ifndef BEEEON_USER_H
#define BEEEON_USER_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"

namespace BeeeOn {

/**
 * Representation of the User application entity.
 */
class User : public Entity {
public:
	typedef Poco::SharedPtr<User> Ptr;
	typedef long ID;

	User(const std::string &data):
		Entity(data)
	{
	}

	User(const User &user):
		Entity(user)
	{
	}

	const std::string email() const
	{
		return get("email");
	}

	const User::ID id() const
	{
		return get<User::ID>("id");
	}
};

typedef User::Collection UserCollection;
typedef User::ID UserID;

}

#endif
