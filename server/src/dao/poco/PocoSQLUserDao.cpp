#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>

#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLUserDao, BeeeOn::PocoSQLUserDao)

void PocoSQLUserDao::create(User &user)
{
	Session session(manager().pool().get());
	create(session, user);
}

bool PocoSQLUserDao::fetch(User &user)
{
	Session session(manager().pool().get());
	return fetch(session, user);
}

void PocoSQLUserDao::create(Session &session, User &user)
{
	user = User(UserID::random(), user);
	string id(user.id().toString());
	string firstName(user.firstName());
	string lastName(user.lastName());

	Statement sql(session);
	sql << "INSERT INTO users"
		" (id, first_name, last_name)"
		" VALUES"
		" (:id, :first_name, :last_name)",
		use(id, "id"),
		use(firstName),
		use(lastName);

	execute(sql);
}

bool PocoSQLUserDao::fetch(Session &session, User &user)
{
	assureHasId(user);
	string id(user.id().toString());
	string firstName;
	string lastName;

	Statement sql(session);
	sql << "SELECT first_name, last_name FROM users"
		" WHERE id = :id",
		use(id, "id"),
		into(firstName),
		into(lastName);

	if (execute(sql) == 0)
		return false;

	user.setFirstName(firstName);
	user.setLastName(lastName);
	return true;
}
