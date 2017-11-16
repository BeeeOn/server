#include <Poco/String.h>

#include "server/AccessLevel.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

AccessLevel::AccessLevel():
	m_value(NONE)
{
}

AccessLevel::AccessLevel(unsigned int level):
	m_value(level)
{
}

AccessLevel::AccessLevel(const AccessLevel &level):
	m_value(level.m_value)
{
}

const string AccessLevel::toString() const
{
	if (m_value <= ANY)
		return "any";

	if (m_value <= ADMIN)
		return "admin";

	if (m_value <= USER)
		return "user";

	if (m_value <= GUEST)
		return "guest";

	return "none";
}

const AccessLevel &AccessLevel::any()
{
	static AccessLevel level(ANY);
	return level;
}

const AccessLevel &AccessLevel::admin()
{
	static AccessLevel level(ADMIN);
	return level;
}

const AccessLevel &AccessLevel::user()
{
	static AccessLevel level(USER);
	return level;
}

const AccessLevel &AccessLevel::guest()
{
	static AccessLevel level(GUEST);
	return level;
}

const AccessLevel &AccessLevel::none()
{
	static AccessLevel level(NONE);
	return level;
}

const AccessLevel &AccessLevel::parse(const string &level)
{

	if (icompare(level, "any") == 0)
		return any();
	else if (icompare(level, "admin") == 0)
		return admin();
	else if (icompare(level, "user") == 0)
		return user();
	else if (icompare(level, "guest") == 0)
		return guest();
	else
		return none();
}
