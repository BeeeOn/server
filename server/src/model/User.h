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
		m_id(copy.m_id),
		m_email(copy.m_email),
		m_firstName(copy.m_firstName),
		m_lastName(copy.m_lastName)
	{
	}

	User(const ID &id, const User &copy):
		m_id(id),
		m_email(copy.m_email),
		m_firstName(copy.m_firstName),
		m_lastName(copy.m_lastName)
	{
	}

	void setFirstName(const std::string &firstName)
	{
		m_firstName = firstName;
	}

	std::string firstName() const
	{
		return m_firstName;
	}

	void setLastName(const std::string &lastName)
	{
		m_lastName = lastName;
	}

	std::string lastName() const
	{
		return m_lastName;
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
	std::string m_firstName;
	std::string m_lastName;
};

typedef User::Collection UserCollection;
typedef User::ID UserID;

}

#endif
