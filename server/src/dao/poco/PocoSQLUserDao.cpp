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

	Statement sql(session);
	sql << "SELECT first_name, last_name FROM users"
		" WHERE id = :id",
		use(id, "id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, user);
}

bool PocoSQLUserDao::parseSingle(RecordSet &result,
		User &user, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), user, prefix);
}

bool PocoSQLUserDao::parseSingle(Row &result,
		User &user, const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		user = User(UserID::parse(result[prefix + "id"]));

	user.setFirstName(result[prefix + "first_name"]);
	user.setLastName(result[prefix + "last_name"]);

	return true;
}

bool PocoSQLUserDao::parseIfIDNotNull(Row &result,
			User &user, const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	user = User(UserID::parse(id));
	user.setFirstName(result[prefix + "first_name"]);
	user.setLastName(result[prefix + "last_name"]);
	return true;
}
