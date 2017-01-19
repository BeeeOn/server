#ifndef BEEEON_IDENTITY_H
#define BEEEON_IDENTITY_H

#include <Poco/SharedPtr.h>

#include "model/Entity.h"
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
class Identity : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<Identity> Ptr;

	Identity();
	Identity(const ID &id);
	Identity(const std::string &email);

	void setEmail(const std::string email)
	{
		m_email = email;
	}

	std::string email() const
	{
		return m_email;
	}

	const std::string toString() const override
	{
		return "'" + m_email + "', '" + id().toString() + "'";
	}

private:
	std::string m_email;
};

typedef Identity::ID IdentityID;

}

#endif
