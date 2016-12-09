#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>

#include "dao/poco/PocoSQLPlaceDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLPlaceDao, BeeeOn::PocoSQLPlaceDao)

void PocoSQLPlaceDao::create(Place &place)
{
	Session session(manager().pool().get());
	create(session, place);
}

bool PocoSQLPlaceDao::fetch(Place &place)
{
	Session session(manager().pool().get());
	return fetch(session, place);
}

bool PocoSQLPlaceDao::update(Place &place)
{
	Session session(manager().pool().get());
	return update(session, place);
}

bool PocoSQLPlaceDao::remove(const Place &place)
{
	Session session(manager().pool().get());
	return remove(session, place);
}

void PocoSQLPlaceDao::create(Session &session, Place &place)
{
	place = Place(PlaceID::random(), place);
	string id(place.id().toString());
	string name(place.name());

	Statement sql(session);
	sql << "INSERT INTO places"
		" (id, name)"
		" VALUES"
		" (:id, :name)",
		use(id, "id"),
		use(name, "name");

	execute(sql);
}

bool PocoSQLPlaceDao::fetch(Session &session, Place &place)
{
	assureHasId(place);
	string id(place.id().toString());
	string name;

	Statement sql(session);
	sql << "SELECT name FROM places"
		" WHERE id = :id",
		use(id, "id"),
		into(name);

	if (execute(sql) == 0)
		return false;

	place.setName(name);
	return true;
}

bool PocoSQLPlaceDao::update(Session &session, Place &place)
{
	assureHasId(place);
	string id(place.id().toString());
	string name(place.name());

	Statement sql(session);
	sql << "UPDATE places"
		" SET name = :name"
		" WHERE id = :id",
		use(name, "name"),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLPlaceDao::remove(Session &session, const Place &place)
{
	assureHasId(place);
	string id(place.id().toString());

	Statement sql(session);
	sql << "DELETE FROM places"
		" WHERE id = :id",
		use(id, "id");

	return execute(sql) > 0;
}
