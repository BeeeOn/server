#ifndef BEEEON_IDENTITY_H
#define BEEEON_IDENTITY_H

#include <Poco/SharedPtr.h>

#include "model/User.h"
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * User identity representation. A user can have a number
 * of identities. An identity can be verified or non-verified.
 * An identity is verified when a user logs into the system
 * by using the identity's email and some 3rd party service
 * (Google, Facebook, ...). This class represents any identity
 * in the system and has a globally unique e-mail.
 */
class Identity {
public:
	typedef Poco::SharedPtr<Identity> Ptr;
	typedef GlobalID ID;

	Identity();
	Identity(const ID &id);
	Identity(const std::string &email);
	Identity(const Identity &copy);
	Identity(const ID &id, const Identity &copy);

	void setEmail(const std::string email)
	{
		m_email = email;
	}

	std::string email() const
	{
		return m_email;
	}

	bool hasUser() const
	{
		return !m_user.id().isNull();
	}

	void setUser(const User &user)
	{
		m_user = user;
	}

	const User &user() const
	{
		return m_user;
	}

	void setID(const ID &id)
	{
		m_id = id;
	}

	const ID &id() const
	{
		return m_id;
	}

	const std::string repr() const
	{
		return "'" + m_email + "', '" + m_id.toString() + "'";
	}

private:
	ID          m_id;
	std::string m_email;
	User        m_user;
};

typedef Identity::ID IdentityID;

}

#endif
