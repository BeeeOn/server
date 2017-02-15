#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>

#include "di/Injectable.h"
#include "dao/TransactionManager.h"
#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLUserDao)
BEEEON_OBJECT_CASTABLE(UserDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLUserDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLUserDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLUserDao::setSQLLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLUserDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLUserDao)

PocoSQLUserDao::PocoSQLUserDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryFetch);
}

void PocoSQLUserDao::create(User &user)
{
	user.setId(UserID::random());
	string id(user.id().toString());
	string firstName(user.firstName());
	string lastName(user.lastName());

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(firstName),
		use(lastName)
	);

	execute(sql);
}

bool PocoSQLUserDao::fetch(User &user)
{
	assureHasId(user);
	string id(user.id().toString());

	Statement sql = (session() << m_queryFetch(),
		use(id, "id")
	);

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
		user.setId(UserID::parse(result[prefix + "id"]));

	user.setFirstName(result[prefix + "first_name"]);
	user.setLastName(result[prefix + "last_name"]);

	markLoaded(user);
	return true;
}

bool PocoSQLUserDao::parseIfIDNotNull(Row &result,
			User &user, const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	user.setId(UserID::parse(id));
	user.setFirstName(result[prefix + "first_name"]);
	user.setLastName(result[prefix + "last_name"]);

	markLoaded(user);
	return true;
}
