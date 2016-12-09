#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RecordSet.h>

#include "dao/poco/PocoSQLRoleInPlaceDao.h"
#include "dao/poco/PocoSQLPlaceDao.h"
#include "dao/poco/PocoSQLIdentityDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "model/User.h"
#include "model/Place.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLRoleInPlaceDao, BeeeOn::PocoSQLRoleInPlaceDao)

void PocoSQLRoleInPlaceDao::create(RoleInPlace &role)
{
	Session session(manager().pool().get());
	create(session, role);
}

bool PocoSQLRoleInPlaceDao::fetch(RoleInPlace &role)
{
	Session session(manager().pool().get());
	return fetch(session, role);
}

void PocoSQLRoleInPlaceDao::fetchBy(
		std::vector<RoleInPlace> &roles,
		const Place &place)
{
	Session session(manager().pool().get());
	fetchBy(session, roles, place);
}

bool PocoSQLRoleInPlaceDao::hasUsersExcept(
		const Place &place, const User &user)
{
	Session session(manager().pool().get());
	return hasUsersExcept(session, place, user);
}

bool PocoSQLRoleInPlaceDao::update(RoleInPlace &role)
{
	Session session(manager().pool().get());
	return update(session, role);
}

bool PocoSQLRoleInPlaceDao::remove(const RoleInPlace &role)
{
	Session session(manager().pool().get());
	return remove(session, role);
}

AccessLevel PocoSQLRoleInPlaceDao::fetchAccessLevel(
		const Place &place,
		const User &user)
{
	Session session(manager().pool().get());
	return fetchAccessLevel(session, place, user);
}

void PocoSQLRoleInPlaceDao::fetchAccessiblePlaces(
		std::vector<Place> &list,
		const User &user,
		const AccessLevel &atLeast)
{
	Session session(manager().pool().get());
	fetchAccessiblePlaces(session, list, user, atLeast);
}

void PocoSQLRoleInPlaceDao::create(Session &session, RoleInPlace &role)
{
	assureHasId(role.place());
	assureHasId(role.identity());

	role = RoleInPlace(RoleInPlaceID::random(), role);
	string id(role.id().toString());
	string placeID(role.place().id().toString());
	string identityID(role.identity().id().toString());
	unsigned int level = role.level();

	Statement sql(session);
	sql << "INSERT INTO roles_in_place"
		" (id, place_id, identity_id, level)"
		" VALUES"
		" (:id, :place_id, :identity_id, :level)",
		use(id, "id"),
		use(placeID, "place_id"),
		use(identityID, "identity_id"),
		use(level, "level");

	execute(sql);
}

bool PocoSQLRoleInPlaceDao::fetch(Session &session, RoleInPlace &role)
{
	assureHasId(role);

	string id(role.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" r.place_id AS place_id,"
		" r.identity_id AS identity_id,"
		" r.level AS level,"
		" i.email AS identity_email,"
		" p.name AS place_name"
		" FROM roles_in_place AS r"
		" JOIN identities AS i ON r.identity_id = i.id"
		" JOIN places AS p ON r.place_id = p.id"
		" WHERE id = :id",
		use(id, "id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, role);
}

void PocoSQLRoleInPlaceDao::fetchBy(Session &session,
		std::vector<RoleInPlace> &roles,
		const Place &place)
{
	assureHasId(place);

	string placeID(place.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" r.id AS id,"
		" r.identity_id AS identity_id,"
		" r.level AS level,"
		" i.email AS identity_email,"
		" r.place_id AS place_id,"
		" p.name AS place_name"
		" FROM roles_in_place AS r"
		" JOIN identities AS i ON r.identity_id = i.id"
		" JOIN places AS p ON r.place_id = p.id"
		" WHERE r.place_id = :place_id",
		use(placeID, "place_id");

	execute(sql);
	RecordSet result(sql);
	parseMany(result, roles);
}

bool PocoSQLRoleInPlaceDao::hasUsersExcept(
		Session &session,
		const Place &place,
		const User &user)
{
	assureHasId(place);
	assureHasId(user);

	string placeID(place.id().toString());
	string userID(user.id().toString());
	unsigned long count;

	Statement sql(session);
	sql << "SELECT COUNT(*)"
		" FROM roles_in_place AS r"
		" JOIN verified_identities AS v"
		"   ON v.identity_id = r.identity_id"
		" WHERE r.place_id = :place_id AND v.user_id <> :user_id",
		use(placeID, "place_id"),
		use(userID, "user_id"),
		into(count);

	execute(sql);
	return count > 0;
}

bool PocoSQLRoleInPlaceDao::update(Session &session, RoleInPlace &role)
{
	assureHasId(role);

	string id(role.id().toString());
	unsigned int level = role.level();

	Statement sql(session);
	sql << "UPDATE roles_in_place"
		" SET level = :level"
		" WHERE id = :id",
		use(level, "level"),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLRoleInPlaceDao::remove(Session &session, const RoleInPlace &role)
{
	assureHasId(role);

	string id(role.id().toString());

	Statement sql(session);
	sql << "DELETE FROM roles_in_place"
		" WHERE id = :id",
		use(id, "id");

	return execute(sql) > 0;
}

AccessLevel PocoSQLRoleInPlaceDao::fetchAccessLevel(
		Session &session,
		const Place &place,
		const User &user)
{
	assureHasId(place);
	assureHasId(user);

	string placeID(place.id().toString());
	string userID(user.id().toString());
	Nullable<unsigned int> level;

	Statement sql(session);
	sql << "SELECT MIN(r.level)"
		" FROM roles_in_place AS r"
		" JOIN verified_identities AS v"
		"   ON v.identity_id = r.identity_id"
		" WHERE r.place_id = :place_id AND v.user_id = :user_id",
		use(placeID, "place_id"),
		use(userID, "user_id"),
		into(level);

	execute(sql);
	return level.isNull()? AccessLevel::none() : AccessLevel(level);
}

void PocoSQLRoleInPlaceDao::fetchAccessiblePlaces(
		Session &session,
		std::vector<Place> &list,
		const User &user,
		const AccessLevel &atLeast)
{
	assureHasId(user);

	string userID(user.id().toString());
	unsigned int level = atLeast;

	Statement sql(session);
	sql << "SELECT"
		" DISTINCT p.id AS id,"
		" p.name AS name"
		" FROM places AS p"
		" JOIN roles_in_place AS r ON r.place_id = p.id"
		" JOIN verified_identities AS v"
		"   ON v.identity_id = r.identity_id"
		" WHERE r.level >= :at_least AND v.user_id = :user_id",
		use(level, "at_least"),
		use(userID, "user_id");

	execute(sql);
	RecordSet result(sql);
	PocoSQLPlaceDao::parseMany(result, list);
}

bool PocoSQLRoleInPlaceDao::parseSingle(RecordSet &result,
		RoleInPlace &role, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), role, prefix);
}

bool PocoSQLRoleInPlaceDao::parseSingle(Row &result,
		RoleInPlace &role, const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		role = RoleInPlace(RoleInPlaceID::parse(result[prefix + "id"]));

	role.setLevel(AccessLevel(result[prefix + "level"].convert<unsigned int>()));

	Place place;
	if (!PocoSQLPlaceDao::parseIfIDNotNull(result, place, prefix + "place_"))
		throw IllegalStateException("place is incomplete in query request");

	role.setPlace(place);

	Identity identity;
	if (!PocoSQLIdentityDao::parseIfIDNotNull(result, identity, prefix + "identity_"))
		throw IllegalStateException("identity is incomplete in query request");

	role.setIdentity(identity);

	return true;
}
