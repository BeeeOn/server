#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RecordSet.h>

#include "dao/poco/PocoSQLVerifiedIdentityDao.h"
#include "dao/poco/PocoSQLIdentityDao.h"
#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLVerifiedIdentityDao, BeeeOn::PocoSQLVerifiedIdentityDao)

void PocoSQLVerifiedIdentityDao::create(VerifiedIdentity &identity)
{
	Session session(manager().pool().get());
	create(session, identity);
}

bool PocoSQLVerifiedIdentityDao::fetch(VerifiedIdentity &identity)
{
	Session session(manager().pool().get());
	return fetch(session, identity);
}

bool PocoSQLVerifiedIdentityDao::fetchBy(
		VerifiedIdentity &identity,
		const string email,
		const string provider)
{
	Session session(manager().pool().get());
	return fetchBy(session, identity, email, provider);
}

void PocoSQLVerifiedIdentityDao::fetchBy(
		std::vector<VerifiedIdentity> &identities,
		const string email)
{
	Session session(manager().pool().get());
	return fetchBy(session, identities, email);
}

bool PocoSQLVerifiedIdentityDao::update(VerifiedIdentity &identity)
{
	Session session(manager().pool().get());
	return update(session, identity);
}

bool PocoSQLVerifiedIdentityDao::remove(const VerifiedIdentity &identity)
{
	Session session(manager().pool().get());
	return remove(session, identity);
}

void PocoSQLVerifiedIdentityDao::create(Session &session,
		VerifiedIdentity &identity)
{
	assureHasId(identity.identity());
	assureHasId(identity.user());

	identity = VerifiedIdentity(VerifiedIdentityID::random(), identity);
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

	Statement sql(session);
	sql << findQuery("verified_identities.create"),
		use(id, "id"),
		use(identityID, "identity_id"),
		use(userID, "user_id"),
		use(provider, "provider"),
		use(picture, "picture"),
		use(accessToken, "access_token");

	execute(sql);
}

bool PocoSQLVerifiedIdentityDao::fetch(Session &session,
		VerifiedIdentity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());

	Statement sql(session);
	sql << findQuery("verified_identities.fetch.by.id"),
		use(id, "id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, identity);
}

bool PocoSQLVerifiedIdentityDao::fetchBy(Session &session,
		VerifiedIdentity &identity,
		const string &email,
		const string &provider)
{
	string searchEmail(email);
	string searchProvider(provider);

	Statement sql(session);
	sql << findQuery("verified_identities.fetch.by.email.and.provider"),
		use(searchEmail, "email"),
		use(searchProvider, "provider");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, identity);
}

void PocoSQLVerifiedIdentityDao::fetchBy(Session &session,
		std::vector<VerifiedIdentity> &identities,
		const string &email)
{
	string searchEmail(email);

	Statement sql(session);
	sql << findQuery("verified_identities.fetch.by.email"),
		use(searchEmail, "email");

	execute(sql);
	RecordSet result(sql);
	parseMany(result, identities);
}

bool PocoSQLVerifiedIdentityDao::update(Session &session,
		VerifiedIdentity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());
	Nullable<string> picture;
	if (!identity.picture().empty())
		picture = identity.picture().toString();

	Nullable<string> accessToken;
	if (!identity.accessToken().empty())
		accessToken = identity.accessToken();

	Statement sql(session);
	sql << findQuery("verified_identities.update"),
		use(picture, "picture"),
		use(accessToken, "access_token"),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLVerifiedIdentityDao::remove(Session &session,
		const VerifiedIdentity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());

	Statement sql(session);
	sql << findQuery("verified_identities.remove"),
		use(id, "id");

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
		identity = VerifiedIdentity(VerifiedIdentityID::parse(result[prefix + "id"]));

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

	return true;
}