#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RecordSet.h>

#include "dao/VerifiedIdentityDao.h"
#include "dao/poco/PocoAbstractDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

namespace BeeeOn {

class PocoSQLVerifiedIdentityDao :
		public PocoAbstractDao,
		public VerifiedIdentityDao {
public:
	void create(VerifiedIdentity &identity) override;
	bool fetch(VerifiedIdentity &identity) override;
	bool fetchBy(VerifiedIdentity &identity,
			const std::string email,
			const std::string provider) override;
	void fetchBy(std::vector<VerifiedIdentity> &identities,
			const std::string email) override;
	bool update(VerifiedIdentity &identity) override;
	bool remove(const VerifiedIdentity &identity) override;

protected:
	void create(Session &session, VerifiedIdentity &identity);
	bool fetch(Session &session, VerifiedIdentity &identity);
	bool fetchBy(Session &session, VerifiedIdentity &identity,
			const std::string &email,
			const std::string &provider);
	void fetchBy(Session &session, 
			std::vector<VerifiedIdentity> &identities,
			const std::string &email);
	bool update(Session &session, VerifiedIdentity &identity);
	bool remove(Session &session, const VerifiedIdentity &identity);
};

}

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
	sql << "INSERT INTO verified_identities ("
		" id,"
		" identity_id,"
		" user_id,"
		" provider,"
		" picture,"
		" access_token"
		")"
		" VALUES ("
		" :id,"
		" :identity_id,"
		" :user_id,"
		" :provider,"
		" :picture,"
		" :access_token"
		")",
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
	string identityID;
	string userID;
	string provider;
	Nullable<string> picture;
	Nullable<string> accessToken;
	string email;
	string firstName;
	string lastName;

	Statement sql(session);
	sql << "SELECT "
		" v.identity_id,"
		" v.user_id,"
		" v.provider,"
		" v.picture,"
		" v.access_token,"
		" i.email,"
		" u.first_name,"
		" u.last_name"
		" FROM verified_identities AS v"
		" JOIN identities AS i ON v.identity_id = i.id"
		" JOIN users AS u ON v.user_id = u.id"
		" WHERE v.id = :id",
		use(id, "id"),
		into(identityID),
		into(userID),
		into(provider),
		into(picture),
		into(accessToken),
		into(email),
		into(firstName),
		into(lastName);

	if (execute(sql) == 0)
		return false;

	User freshUser(UserID::parse(userID));
	freshUser.setFirstName(firstName);
	freshUser.setLastName(lastName);

	Identity frechIdentity(IdentityID::parse(identityID));
	frechIdentity.setEmail(email);

	identity.setUser(freshUser);
	identity.setIdentity(frechIdentity);
	identity.setProvider(provider);
	identity.setPicture(picture.isNull()? URI() : URI(picture));
	identity.setAccessToken(accessToken.isNull()?
			"" : accessToken.value());

	return true;
}

bool PocoSQLVerifiedIdentityDao::fetchBy(Session &session,
		VerifiedIdentity &identity,
		const string &email,
		const string &provider)
{
	string searchEmail(email);
	string searchProvider(provider);

	string id;
	string identityID;
	string userID;
	Nullable<string> picture;
	Nullable<string> accessToken;
	string firstName;
	string lastName;

	Statement sql(session);
	sql << "SELECT "
		" v.id,"
		" v.identity_id,"
		" v.user_id,"
		" v.picture,"
		" v.access_token,"
		" u.first_name,"
		" u.last_name"
		" FROM verified_identities AS v"
		" JOIN identities AS i ON v.identity_id = i.id"
		" JOIN users AS u ON v.user_id = u.id"
		" WHERE i.email = :email AND v.provider = :provider",
		use(searchEmail, "email"),
		use(searchProvider, "provider"),
		into(id),
		into(identityID),
		into(userID),
		into(picture),
		into(accessToken),
		into(firstName),
		into(lastName);

	if (execute(sql) == 0)
		return false;

	User freshUser(UserID::parse(userID));
	freshUser.setFirstName(firstName);
	freshUser.setLastName(lastName);

	Identity frechIdentity(IdentityID::parse(identityID));
	frechIdentity.setEmail(email);

	identity = VerifiedIdentity(VerifiedIdentityID::parse(id));
	identity.setUser(freshUser);
	identity.setIdentity(frechIdentity);
	identity.setProvider(provider);
	identity.setPicture(picture.isNull()? URI() : URI(picture));
	identity.setAccessToken(accessToken.isNull()?
			"" : accessToken.value());

	return true;

}

void PocoSQLVerifiedIdentityDao::fetchBy(Session &session,
		std::vector<VerifiedIdentity> &identities,
		const string &email)
{
	string searchEmail(email);

	Statement sql(session);
	sql << "SELECT "
		" v.id,"
		" v.identity_id,"
		" v.user_id,"
		" v.picture,"
		" v.access_token,"
		" u.first_name,"
		" u.last_name"
		" FROM verified_identities AS v"
		" JOIN identities AS i ON v.identity_id = i.id"
		" JOIN users AS u ON v.user_id = u.id"
		" WHERE i.email = :email",
		use(searchEmail, "email");

	execute(sql);
	RecordSet result(sql);

	for (auto row : result) {
		Identity freshIdentity(IdentityID::parse(row.get(1)));
		freshIdentity.setEmail(email);

		User freshUser(UserID::parse(row.get(2)));
		freshUser.setFirstName(row.get(5));
		freshUser.setLastName(row.get(6));

		VerifiedIdentity identity(
				VerifiedIdentityID::parse(row.get(0)));
		identity.setIdentity(freshIdentity);
		identity.setUser(freshUser);
		identity.setPicture(row.get(3).isEmpty()?
				URI() : URI(row.get(3).toString()));
		identity.setAccessToken(row.get(4).isEmpty()?
				"" : row.get(4));

		identities.push_back(identity);
	}
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
	sql << "UPDATE verified_identities SET"
		" picture = :picture,"
		" access_token = :access_token"
		" WHERE id = :id",
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
	sql << "DELETE FROM verified_identities"
		" WHERE id = :id",
		use(id, "id");

	return execute(sql) > 0;
}
