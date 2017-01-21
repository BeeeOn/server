#ifndef BEEEON_USER_H
#define BEEEON_USER_H

#include <Poco/SharedPtr.h>
#include "model/Entity.h"
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * Representation of the User application entity.
 */
class User : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<User> Ptr;

	User()
	{
	}

	User(const ID &id):
		Entity(id)
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

private:
	std::string m_firstName;
	std::string m_lastName;
};

typedef User::ID UserID;

}

#endif
