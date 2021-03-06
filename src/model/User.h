#pragma once

#include <string>

#include <Poco/SharedPtr.h>

#include "l10n/Locale.h"
#include "model/Entity.h"
#include "model/GlobalID.h"

namespace BeeeOn {

/**
 * Representation of the User application entity.
 */
class User : public Entity<GlobalID> {
public:
	typedef Poco::SharedPtr<User> Ptr;

	User();
	User(const ID &id);

	void setFirstName(const std::string &firstName);
	std::string firstName() const;

	void setLastName(const std::string &lastName);
	std::string lastName() const;

	std::string fullName() const;

	void setLocale(const Locale &locale);
	const Locale &locale() const;

private:
	std::string m_firstName;
	std::string m_lastName;
	Locale m_locale;
};

typedef User::ID UserID;

}
