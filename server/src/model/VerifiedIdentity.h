#ifndef BEEEON_VERIFIED_IDENTITY_H
#define BEEEON_VERIFIED_IDENTITY_H

#include <string>
#include <Poco/URI.h>
#include <Poco/SharedPtr.h>
#include <Poco/Exception.h>

#include "model/GlobalID.h"
#include "model/Identity.h"

namespace BeeeOn {

/**
 * Represents a verified identity, i.e. there is an authentication
 * provider (Google, Facebook) associated with this identity. A verified
 * identity is always assigned to a user. There can be more verified
 * identities with the same e-mail (multiple authentication services
 * created for the same e-mail address).
 */
class VerifiedIdentity {
public:
	typedef Poco::SharedPtr<VerifiedIdentity> Ptr;
	typedef GlobalID ID;

	VerifiedIdentity();
	VerifiedIdentity(const ID &id);
	VerifiedIdentity(const VerifiedIdentity &copy);
	VerifiedIdentity(const ID &id, const VerifiedIdentity &copy);

	void setProvider(const std::string &provider)
	{
		m_provider = provider;
	}

	std::string provider() const
	{
		return m_provider;
	}

	void setAccessToken(const std::string &accessToken)
	{
		m_accessToken = accessToken;
	}

	std::string accessToken() const
	{
		return m_accessToken;
	}

	void setPicture(const Poco::URI &uri)
	{
		m_picture = uri;
	}

	const Poco::URI &picture() const
	{
		return m_picture;
	}

	void setIdentity(const Identity &identity)
	{
		m_identity = identity;
	}

	const Identity &identity() const
	{
		return m_identity;
	}

	void setEmail(const std::string &email)
	{
		m_identity.setEmail(email);
	}

	std::string email() const
	{
		return m_identity.email();
	}

	void setUser(const User &user)
	{
		m_identity.setUser(user);
	}

	const User &user() const
	{
		if (!m_identity.hasUser())
			throw Poco::IllegalStateException("missing a user");

		return m_identity.user();
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
	ID          m_id;
	std::string m_provider;
	std::string m_accessToken;
	Poco::URI   m_picture;
	Identity    m_identity;
};

typedef VerifiedIdentity::ID VerifiedIdentityID;

}

#endif
