#include <Poco/Exception.h>
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

PocoSQLRoleInPlaceDao::PocoSQLRoleInPlaceDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByPlaceId);
	registerQuery(m_queryFetchAccessLevel);
	registerQuery(m_queryCountUsersExcept);
	registerQuery(m_queryFetchAccessiblePlaces);
}

void PocoSQLRoleInPlaceDao::create(RoleInPlace &role)
{
	assureHasId(role.place());
	assureHasId(role.identity());

	role = RoleInPlace(RoleInPlaceID::random(), role);
	string id(role.id().toString());
	string placeID(role.place().id().toString());
	string identityID(role.identity().id().toString());
	unsigned int level = role.level();
	unsigned long created = role.created().timestamp().epochTime();

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(placeID, "place_id"),
		use(identityID, "identity_id"),
		use(level, "level"),
		use(created, "created")
	);

	execute(sql);
}

bool PocoSQLRoleInPlaceDao::fetch(RoleInPlace &role)
{
	assureHasId(role);

	string id(role.id().toString());

	Statement sql = (session() << m_queryFetchById(),
		use(id, "id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, role);
}

void PocoSQLRoleInPlaceDao::fetchBy(std::vector<RoleInPlace> &roles,
		const Place &place)
{
	assureHasId(place);

	string placeID(place.id().toString());

	Statement sql = (session() << m_queryFetchByPlaceId(),
		use(placeID, "place_id")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany(result, roles);
}

bool PocoSQLRoleInPlaceDao::hasUsersExcept(const Place &place, const User &user)
{
	assureHasId(place);
	assureHasId(user);

	string placeID(place.id().toString());
	string userID(user.id().toString());
	unsigned long count;

	Statement sql = (session() << m_queryCountUsersExcept(),
		use(placeID, "place_id"),
		use(userID, "user_id"),
		into(count)
	);

	execute(sql);
	return count > 0;
}

bool PocoSQLRoleInPlaceDao::update(RoleInPlace &role)
{
	assureHasId(role);

	string id(role.id().toString());
	unsigned int level = role.level();

	Statement sql = (session() << m_queryUpdate(),
		use(level, "level"),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLRoleInPlaceDao::remove(const RoleInPlace &role)
{
	assureHasId(role);

	string id(role.id().toString());

	Statement sql = (session() << m_queryRemove(),
		use(id, "id")
	);

	return execute(sql) > 0;
}

AccessLevel PocoSQLRoleInPlaceDao::fetchAccessLevel(
		const Place &place,
		const User &user)
{
	assureHasId(place);
	assureHasId(user);

	string placeID(place.id().toString());
	string userID(user.id().toString());
	Nullable<unsigned int> level;

	Statement sql = (session() << m_queryFetchAccessLevel(),
		use(placeID, "place_id"),
		use(userID, "user_id"),
		into(level)
	);

	execute(sql);
	return level.isNull()? AccessLevel::none() : AccessLevel(level);
}

void PocoSQLRoleInPlaceDao::fetchAccessiblePlaces(
		std::vector<Place> &list,
		const User &user,
		const AccessLevel &atLeast)
{
	assureHasId(user);

	string userID(user.id().toString());
	unsigned int level = atLeast;

	Statement sql = (session() << m_queryFetchAccessiblePlaces(),
		use(level, "at_least"),
		use(userID, "user_id")
	);

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
		role.setId(RoleInPlaceID::parse(result[prefix + "id"]));

	role.setLevel(AccessLevel(result[prefix + "level"].convert<unsigned int>()));
	role.setCreated(Timestamp::fromEpochTime(result[prefix + "created"]));

	Place place;
	if (!PocoSQLPlaceDao::parseIfIDNotNull(result, place, prefix + "place_"))
		throw IllegalStateException("place is incomplete in query request");

	role.setPlace(place);

	Identity identity;
	if (!PocoSQLIdentityDao::parseIfIDNotNull(result, identity, prefix + "identity_"))
		throw IllegalStateException("identity is incomplete in query request");

	role.setIdentity(identity);

	markLoaded(role);
	return true;
}
