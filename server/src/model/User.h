#ifndef BEEEON_USER_H
#define BEEEON_USER_H

#include <Poco/SharedPtr.h>
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * Representation of the User application entity.
 */
class User {
public:
	typedef Poco::SharedPtr<User> Ptr;
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
		m_firstName(copy.m_firstName),
		m_lastName(copy.m_lastName)
	{
	}

	User(const ID &id, const User &copy):
		m_id(id),
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

	std::string fullName() const
	{
		if (m_firstName.empty() && m_lastName.empty())
			return "";

		return m_firstName + " " + m_lastName;
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
	std::string m_firstName;
	std::string m_lastName;
};

typedef User::ID UserID;

}

#endif
