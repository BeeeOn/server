#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RecordSet.h>

#include "dao/RoleInPlaceDao.h"
#include "dao/PocoAbstractDao.h"
#include "dao/PocoDaoManager.h"
#include "model/User.h"
#include "model/Place.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

namespace BeeeOn {

class PocoSQLRoleInPlaceDao :
		public PocoAbstractDao,
		public RoleInPlaceDao {
public:
	void create(RoleInPlace &role) override;
	bool fetch(RoleInPlace &role) override;
	void fetchBy(std::vector<RoleInPlace> &roles,
			const Place &place) override;
	bool hasUsersExcept(const Place &place, const User &user) override;
	bool update(RoleInPlace &role) override;
	bool remove(const RoleInPlace &role) override;

	AccessLevel fetchAccessLevel(
			const Place &place,
			const User &user) override;

	void fetchAccessiblePlaces(
			std::vector<Place> &list,
			const User &user,
			const AccessLevel &atLeast = AccessLevel::any()) override;

protected:
	void create(Session &session, RoleInPlace &role);
	bool fetch(Session &session, RoleInPlace &role);
	void fetchBy(Session &session,
			std::vector<RoleInPlace> &roles,
			const Place &place);
	bool hasUsersExcept(Session &session,
			const Place &place,
			const User &user);
	bool update(Session &session, RoleInPlace &role);
	bool remove(Session &session, const RoleInPlace &role);

	AccessLevel fetchAccessLevel(Session &session,
			const Place &place,
			const User &user);

	void fetchAccessiblePlaces(Session &session,
			std::vector<Place> &list,
			const User &user,
			const AccessLevel &atLeast);
};

}

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
	string placeID;
	string placeName;
	string identityID;
	string email;
	unsigned int level;

	Statement sql(session);
	sql << "SELECT"
		" r.place_id,"
		" r.identity_id,"
		" r.level,"
		" i.email,"
		" p.name"
		" FROM roles_in_place AS r"
		" JOIN identities AS i ON r.identity_id = i.id"
		" JOIN places AS p ON r.place_id = p.id"
		" WHERE id = :id",
		use(id, "id"),
		into(placeID),
		into(identityID),
		into(level),
		into(email),
		into(placeName);

	if (execute(sql) == 0)
		return false;

	Place place(PlaceID::parse(placeID));
	place.setName(placeName);

	Identity identity(IdentityID::parse(identityID));
	identity.setEmail(email);

	role.setPlace(place);
	role.setIdentity(identity);
	role.setLevel(level);

	return true;
}

void PocoSQLRoleInPlaceDao::fetchBy(Session &session,
		std::vector<RoleInPlace> &roles,
		const Place &place)
{
	assureHasId(place);

	string placeID(place.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" r.id,"
		" r.identity_id,"
		" r.level,"
		" i.email,"
		" p.name"
		" FROM roles_in_place AS r"
		" JOIN identities AS i ON r.identity_id = i.id"
		" JOIN places AS p ON r.place_id = p.id"
		" WHERE r.place_id = :place_id",
		use(placeID, "place_id");

	execute(sql);
	RecordSet result(sql);

	for (auto row : result) {
		Place freshPlace(place.id());
		freshPlace.setName(row.get(4));

		Identity identity(IdentityID::parse(row.get(1)));
		identity.setEmail(row.get(3));
		
		RoleInPlace role(RoleInPlaceID::parse(row.get(0)));
		role.setPlace(place);
		role.setIdentity(identity);
		role.setLevel(AccessLevel((int) row.get(2)));

		roles.push_back(role);
	}
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
		" DISTINCT p.id,"
		" p.name"
		" FROM places AS p"
		" JOIN roles_in_place AS r ON r.place_id = p.id"
		" JOIN verified_identities AS v"
		"   ON v.identity_id = r.identity_id"
		" WHERE r.level >= :at_least AND v.user_id = :user_id",
		use(level, "at_least"),
		use(userID, "user_id");

	execute(sql);
	RecordSet result(sql);

	for (auto row : result) {
		Place place(PlaceID::parse(row.get(0)));
		place.setName(row.get(1));

		list.push_back(place);
	}
}
