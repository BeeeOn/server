#ifndef BEEEON_USER_H
#define BEEEON_USER_H

#include <Poco/SharedPtr.h>
#include "model/GlobalID.h"
#include "model/Collection.h"

namespace BeeeOn {

/**
 * Representation of the User application entity.
 */
class User {
public:
	typedef Poco::SharedPtr<User> Ptr;
	typedef BeeeOn::Collection<User> Collection;
	typedef GlobalID ID;

	User()
	{
	}

	User(const ID &id):
		m_id(id)
	{
	}

	User(const User &copy):
		m_email(copy.m_email)
	{
	}

	User(const ID &id, const User &copy):
		m_id(id),
		m_email(copy.m_email)
	{
	}

	void setEmail(const std::string &email)
	{
		m_email = email;
	}

	const std::string email() const
	{
		return m_email;
	}

	const ID &id() const
	{
		return m_id;
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void toWeb(Serializer &s) const
	{
		s.push("email", m_email);
	}

	/**
	 * Frontend API only.
	 */
	template <typename Serializer>
	void fromWeb(Serializer &s)
	{
		s.get("email", m_email);
	}

private:
	ID m_id;
	std::string m_email;
};

typedef User::Collection UserCollection;
typedef User::ID UserID;

}

#endif
