#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>

#include "dao/poco/PocoSQLIdentityDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLIdentityDao, BeeeOn::PocoSQLIdentityDao)

PocoSQLIdentityDao::PocoSQLIdentityDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByEmail);
}

void PocoSQLIdentityDao::create(Identity &identity)
{
	Session session(manager().pool().get());
	create(session, identity);
}

bool PocoSQLIdentityDao::fetch(Identity &identity)
{
	Session session(manager().pool().get());
	return fetch(session, identity);
}

bool PocoSQLIdentityDao::fetchBy(Identity &identity, const string &email)
{
	Session session(manager().pool().get());
	return fetchBy(session, identity, email);
}

bool PocoSQLIdentityDao::remove(const Identity &identity)
{
	Session session(manager().pool().get());
	return remove(session, identity);
}

void PocoSQLIdentityDao::create(Session &session, Identity &identity)
{
	identity = Identity(IdentityID::random(), identity);
	string id(identity.id().toString());
	string email(identity.email());

	Statement sql(session);
	sql << m_queryCreate(),
		use(id, "id"),
		use(email, "email");

	execute(sql);
}

bool PocoSQLIdentityDao::fetch(Session &session, Identity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());

	Statement sql(session);
	sql << m_queryFetchById(),
		use(id, "id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, identity);
}

bool PocoSQLIdentityDao::fetchBy(Session &session,
		Identity &identity, const string &email)
{
	string searchEmail(email);
	string id;

	Statement sql(session);
	sql << m_queryFetchByEmail(),
		use(searchEmail, "email");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, identity);
}

bool PocoSQLIdentityDao::remove(Session &session, const Identity &identity)
{
	assureHasId(identity);
	string id(identity.id().toString());

	Statement sql(session);
	sql << m_queryRemove(),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLIdentityDao::parseSingle(RecordSet &result,
		Identity &identity, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), identity, prefix);
}

bool PocoSQLIdentityDao::parseSingle(Row &result,
		Identity &identity, const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		identity = Identity(IdentityID::parse(result[prefix + "id"]));

	identity.setEmail(result[prefix + "email"]);
	return true;
}

bool PocoSQLIdentityDao::parseIfIDNotNull(Row &result,
			Identity &identity, const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	identity = Identity(IdentityID::parse(id));
	identity.setEmail(result[prefix + "email"]);
	return true;
}
