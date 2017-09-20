#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RecordSet.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLVerifiedIdentityDao.h"
#include "dao/poco/PocoSQLIdentityDao.h"
#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLVerifiedIdentityDao)
BEEEON_OBJECT_CASTABLE(VerifiedIdentityDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLVerifiedIdentityDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLVerifiedIdentityDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLVerifiedIdentityDao::setSQLLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLVerifiedIdentityDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLVerifiedIdentityDao)

PocoSQLVerifiedIdentityDao::PocoSQLVerifiedIdentityDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByEmail);
	registerQuery(m_queryFetchByEmailAndProvider);
	registerQuery(m_queryFetchByUser);
}

void PocoSQLVerifiedIdentityDao::create(VerifiedIdentity &identity)
{
	assureHasId(identity.identity());
	assureHasId(identity.user());

	identity.setId(VerifiedIdentityID::random());
	string id(identity.id().toString());
	string identityID(identity.identity().id().toString());
	string userID(identity.user().id().toString());
	string provider(identity.provider());

	Nullable<string> picture;
	if (!identity.picture().empty())
		picture = identity.picture().toString();

	Nullable<string> accessToken;
	if (!identity.accessToken().empty())
		accessToken = identity.accessToken();

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(identityID, "identity_id"),
		use(userID, "user_id"),
		use(provider, "provider"),
		use(picture, "picture"),
		use(accessToken, "access_token")
	);

	execute(sql);
}

bool PocoSQLVerifiedIdentityDao::fetch(VerifiedIdentity &identity)
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

bool PocoSQLVerifiedIdentityDao::fetchBy(
		VerifiedIdentity &identity,
		const string email,
		const string provider)
{
	string searchEmail(email);
	string searchProvider(provider);

	Statement sql = (session() << m_queryFetchByEmailAndProvider(),
		use(searchEmail, "email"),
		use(searchProvider, "provider")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, identity);
}

void PocoSQLVerifiedIdentityDao::fetchBy(
		std::vector<VerifiedIdentity> &identities,
		const string email)
{
	string searchEmail(email);

	Statement sql = (session() << m_queryFetchByEmail(),
		use(searchEmail, "email")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany(result, identities);
}

void PocoSQLVerifiedIdentityDao::fetchBy(
		std::list<VerifiedIdentity> &identities,
		const User &user)
{
	assureHasId(user);

	string id(user.id().toString());

	Statement sql = (session() << m_queryFetchByUser(),
		use(id, "user_id")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany(result, identities);
}

bool PocoSQLVerifiedIdentityDao::update(VerifiedIdentity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());
	Nullable<string> picture;
	if (!identity.picture().empty())
		picture = identity.picture().toString();

	Nullable<string> accessToken;
	if (!identity.accessToken().empty())
		accessToken = identity.accessToken();

	Statement sql = (session() << m_queryUpdate(),
		use(picture, "picture"),
		use(accessToken, "access_token"),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLVerifiedIdentityDao::remove(const VerifiedIdentity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());

	Statement sql = (session() << m_queryRemove(),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLVerifiedIdentityDao::parseSingle(RecordSet &result,
		VerifiedIdentity &identity, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), identity, prefix);
}

bool PocoSQLVerifiedIdentityDao::parseSingle(Row &result,
		VerifiedIdentity &identity, const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		identity.setId(VerifiedIdentityID::parse(result[prefix + "id"]));

	const Poco::Dynamic::Var &picture = result[prefix + "picture"];
	identity.setPicture(picture.isEmpty()? URI() : URI(picture.toString()));

	identity.setProvider(result[prefix + "provider"]);
	identity.setAccessToken(emptyWhenNull(result[prefix + "access_token"]));

	Identity id;
	if (!PocoSQLIdentityDao::parseIfIDNotNull(result, id, prefix + "identity_"))
		throw IllegalStateException("identity is incomplete in query result");

	identity.setIdentity(id);

	User user;
	if (!PocoSQLUserDao::parseIfIDNotNull(result, user, prefix + "user_"))
		throw IllegalStateException("user is incomplete in query result");

	identity.setUser(user);

	markLoaded(identity);
	return true;
}
