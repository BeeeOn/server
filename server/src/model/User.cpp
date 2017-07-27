#include "model/User.h"

using namespace std;
using namespace BeeeOn;

User::User()
{
}

User::User(const ID &id):
	Entity(id)
{
}

void User::setFirstName(const string &firstName)
{
	m_firstName = firstName;
}

string User::firstName() const
{
	return m_firstName;
}

void User::setLastName(const string &lastName)
{
	m_lastName = lastName;
}

string User::lastName() const
{
	return m_lastName;
}

string User::fullName() const
{
	if (m_firstName.empty() && m_lastName.empty())
		return "";

	return m_firstName + " " + m_lastName;
}

void User::setLocale(const Locale &locale)
{
	m_locale = locale;
}

const Locale &User::locale() const
{
	return m_locale;
}
