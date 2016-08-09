#include "dao/UserDao.h"

BEEEON_OBJECT(MockUserDao, BeeeOn::MockUserDao)
BEEEON_OBJECT(NullUserDao, BeeeOn::NullUserDao)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

bool MockUserDao::fetch(User &user)
{
	if (!user.id().isNull())
		return MockDao<User, UserDao>::fetch(user);

	if (user.email().empty())
		throw InvalidArgumentException("missing e-mail or ID");

	Iterator it = findByEmail(user.email());
	if (it == storage().end())
		return false;

	user = User(it->first, *it->second);
	return true;
}

bool MockUserDao::has(const User &user)
{
	if (!user.id().isNull())
		return MockDao<User, UserDao>::has(user);

	if (user.email().empty())
		throw InvalidArgumentException("missing e-mail or ID");

	Iterator it = findByEmail(user.email());
	for (; it != storage().end(); ++it) {
		if (it->second->email() == user.email())
			return true;
	}

	return false;
}

void MockUserDao::create(User &user)
{
	if (user.email().empty())
		throw InvalidArgumentException("missing e-mail");

	MockDao<User, UserDao>::create(user);
}

bool MockUserDao::update(User &user)
{
	if (!user.id().isNull()) {
		Iterator it = storage().find(user.id());
		// ensure e-mail is set
		user.setEmail(it->second->email());
		return MockDao<User, UserDao>::update(user);
	}

	if (user.email().empty())
		throw InvalidArgumentException("missing e-mail or ID");

	Iterator it = findByEmail(user.email());
	if (it == storage().end())
		return false;

	// ensure right ID
	User copy(it->first, user);

	if (!MockDao<User, UserDao>::update(copy))
		return false;

	// get fresh data
	user = copy;
	return true;
}

bool MockUserDao::remove(const User &user)
{
	if (!user.id().isNull())
		return MockDao<User, UserDao>::remove(user);

	if (user.email().empty())
		throw InvalidArgumentException("missing e-mail or ID");

	Iterator it = findByEmail(user.email());
	if (it == storage().end())
		return false;

	storage().erase(it);
	return true;
}

MockUserDao::Iterator MockUserDao::findByEmail(const string &email)
{
	Iterator it = storage().begin();
	for (; it != storage().end(); ++it) {
		if (it->second->email() == email)
			break;
	}

	return it;
}
