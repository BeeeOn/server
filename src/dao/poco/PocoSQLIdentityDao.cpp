#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLIdentityDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLIdentityDao)
BEEEON_OBJECT_CASTABLE(IdentityDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLIdentityDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLIdentityDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLIdentityDao::setQueryLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLIdentityDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLIdentityDao)

PocoSQLIdentityDao::PocoSQLIdentityDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByEmail);
}

void PocoSQLIdentityDao::create(Identity &identity)
{
	identity.setId(IdentityID::random());
	string id(identity.id().toString());
	string email(identity.email());

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(email, "email")
	);

	execute(sql);
}

bool PocoSQLIdentityDao::fetch(Identity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());

	Statement sql = (session() << m_queryFetchById(),
		use(id, "id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, identity);
}

bool PocoSQLIdentityDao::fetchBy(Identity &identity, const string &email)
{
	string searchEmail(email);
	string id;

	Statement sql = (session() << m_queryFetchByEmail(),
		use(searchEmail, "email")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, identity);
}

bool PocoSQLIdentityDao::remove(const Identity &identity)
{
	assureHasId(identity);
	string id(identity.id().toString());

	Statement sql = (session() << m_queryRemove(),
		use(id, "id")
	);

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
		identity.setId(IdentityID::parse(result[prefix + "id"]));

	identity.setEmail(result[prefix + "email"]);

	markLoaded(identity);
	return true;
}

bool PocoSQLIdentityDao::parseIfIDNotNull(Row &result,
			Identity &identity, const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	identity.setId(IdentityID::parse(id));
	identity.setEmail(result[prefix + "email"]);

	markLoaded(identity);
	return true;
}
