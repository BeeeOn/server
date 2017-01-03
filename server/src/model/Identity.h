#ifndef BEEEON_IDENTITY_H
#define BEEEON_IDENTITY_H

#include <Poco/SharedPtr.h>

#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * Non-verified identity representation. Before a user is registered
 * in the system, it is possible to represent it as an identity.
 * An identity can be verified or non-verified.
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
};

typedef Identity::ID IdentityID;

}

#endif