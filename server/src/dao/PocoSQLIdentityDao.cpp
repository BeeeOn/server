#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>

#include "dao/IdentityDao.h"
#include "dao/PocoAbstractDao.h"
#include "dao/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

namespace BeeeOn {

class PocoSQLIdentityDao :
		public PocoAbstractDao,
		public IdentityDao {
public:
	void create(Identity &identity) override;
	bool fetch(Identity &identity) override;
	bool fetchBy(Identity &identity,
			const std::string &email) override;
	bool remove(const Identity &identity) override;

protected:
	void create(Session &session, Identity &identity);
	bool fetch(Session &session, Identity &identity);
	bool fetchBy(Session &session, Identity &identity,
			const std::string &email);
	bool remove(Session &session, const Identity &identity);
};

}

BEEEON_OBJECT(PocoSQLIdentityDao, BeeeOn::PocoSQLIdentityDao)

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
	sql << "INSERT INTO identities"
		" (id, email)"
		" VALUES"
		" (:id, :email)",
		use(id, "id"),
		use(email, "email");

	execute(sql);
}

bool PocoSQLIdentityDao::fetch(Session &session, Identity &identity)
{
	assureHasId(identity);

	string id(identity.id().toString());
	string email;

	Statement sql(session);
	sql << "SELECT email FROM identities"
		" WHERE id = :id",
		use(id, "id"),
		into(email);

	if (execute(sql) == 0)
		return false;

	identity.setEmail(email);
	return true;
}

bool PocoSQLIdentityDao::fetchBy(Session &session,
		Identity &identity, const string &email)
{
	string searchEmail(email);
	string id;

	Statement sql(session);
	sql << "SELECT id FROM identities"
		" WHERE email = :email",
		use(searchEmail, "email"),
		into(id);

	if (execute(sql) == 0)
		return false;

	identity = Identity(IdentityID::parse(id));
	identity.setEmail(email);
	return true;
}

bool PocoSQLIdentityDao::remove(Session &session, const Identity &identity)
{
	assureHasId(identity);
	string id(identity.id().toString());

	Statement sql(session);
	sql << "DELETE FROM identities"
		" WHERE id = :id",
		use(id, "id");

	return execute(sql) > 0;
}
