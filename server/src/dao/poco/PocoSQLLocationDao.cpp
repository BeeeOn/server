#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "dao/poco/PocoSQLLocationDao.h"
#include "dao/poco/PocoSQLPlaceDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLLocationDao, BeeeOn::PocoSQLLocationDao)

void PocoSQLLocationDao::create(Location &location)
{
	Session session(manager().pool().get());
	create(session, location);
}

bool PocoSQLLocationDao::fetch(Location &location)
{
	Session session(manager().pool().get());
	return fetch(session, location);
}

bool PocoSQLLocationDao::fetchFrom(Location &location,
		const Place &place)
{
	Session session(manager().pool().get());
	return fetchFrom(session, location, place);
}

bool PocoSQLLocationDao::fetchFrom(Location &location,
		const Gateway &gateway)
{
	Session session(manager().pool().get());
	return fetchFrom(session, location, gateway);
}

void PocoSQLLocationDao::fetchBy(std::vector<Location> &locations,
		const Place &place)
{
	Session session(manager().pool().get());
	fetchBy(session, locations, place);
}

void PocoSQLLocationDao::fetchBy(std::vector<Location> &locations,
		const Gateway &gateway)
{
	Session session(manager().pool().get());
	fetchBy(session, locations, gateway);
}

bool PocoSQLLocationDao::update(Location &location)
{
	Session session(manager().pool().get());
	return update(session, location);
}

bool PocoSQLLocationDao::remove(const Location &location)
{
	Session session(manager().pool().get());
	return remove(session, location);
}

void PocoSQLLocationDao::create(Session &session,
		Location &location)
{
	location = Location(LocationID::random(), location);
	string id(location.id().toString());
	string name(location.name());
	string placeID(location.place().id().toString());

	Statement sql(session);
	sql << "INSERT INTO locations"
		" (id, name, place_id)"
		" VALUES"
		" (:id, :name, :place_id)",
		use(id, "id"),
		use(name, "name"),
		use(placeID, "place_id");

	execute(sql);
}

bool PocoSQLLocationDao::fetch(Session &session,
		Location &location)
{
	assureHasId(location);
	string id(location.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" l.name AS name,"
		" p.name AS place_name,"
		" p.id AS place_id"
		" FROM locations AS l"
		" JOIN places AS p ON l.place_id = p.id"
		" WHERE l.id = :id",
		use(id, "id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, location);
}

bool PocoSQLLocationDao::fetchFrom(Session &session,
		Location &location, const Place &place)
{
	assureHasId(location);
	assureHasId(place);

	string id(location.id().toString());
	string placeID(place.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" l.name AS name,"
		" p.name AS place_name,"
		" l.place_id AS place_id"
		" FROM locations AS l"
		" JOIN places AS p ON l.place_id = p.id"
		" WHERE l.id = :id AND p.id = :place_id",
		use(id, "id"),
		use(placeID, "place_id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, location);
}

bool PocoSQLLocationDao::fetchFrom(Session &session,
		Location &location, const Gateway &gateway)
{
	assureHasId(location);
	assureHasId(gateway);

	string id(location.id().toString());
	string gatewayID(gateway.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" l.name AS name,"
		" p.name AS place_name,"
		" p.id AS place_id"
		" FROM locations AS l"
		" JOIN places AS p ON l.place_id = p.id"
		" JOIN gateways AS g ON g.place_id = p.id"
		" WHERE l.id = :id AND g.id = :gateway_id",
		use(id, "id"),
		use(gatewayID, "gateway_id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, location);
}

void PocoSQLLocationDao::fetchBy(Session &session,
		std::vector<Location> &locations,
		const Place &place)
{
	assureHasId(place);

	string placeID(place.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" l.id AS id,"
		" l.name AS name,"
		" l.place_id AS place_id,"
		" p.name AS place_name"
		" FROM locations AS l"
		" JOIN places AS p ON p.id = l.place_id"
		" WHERE place_id = :place_id",
		use(placeID, "place_id");

	execute(sql);
	RecordSet result(sql);

	for (auto row : result) {
		Location location;

		if (!parseSingle(row, location)) {
			m_logger.warning("skipping malformed location, query result: "
					+ row.valuesToString(), __FILE__, __LINE__)
			continue;
		}

		locations.push_back(location);
	}
}

void PocoSQLLocationDao::fetchBy(Session &session,
		std::vector<Location> &locations,
		const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" l.id as id,"
		" l.name as name,"
		" l.place_id as place_id,"
		" p.name as place_name"
		" FROM locations AS l"
		" JOIN gateways AS g ON g.place_id = l.place_id"
		" JOIN places AS p ON p.id = l.place_id"
		" WHERE g.id = :gateway_id",
		use(gatewayID, "gateway_id");

	execute(sql);
	RecordSet result(sql);

	for (auto row : result) {
		Location location;

		if (!parseSingle(row, location)) {
			m_logger.warning("skipping malformed location, query result: "
					+ row.valuesToString(), __FILE__, __LINE__)
			continue;
		}

		locations.push_back(location);
	}
}

bool PocoSQLLocationDao::update(Session &session,
		Location &location)
{
	assureHasId(location);
	assureHasId(location.place());

	string id(location.id().toString());
	string name(location.name());
	string placeID(location.place().id().toString());

	Statement sql(session);
	sql << "UPDATE locations"
		" SET name = :name"
		" WHERE id = :id",
		use(name, "name"),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLLocationDao::remove(Session &session, const Location &location)
{
	assureHasId(location);
	string id(location.id().toString());

	Statement sql(session);
	sql << "DELETE FROM locations"
		" WHERE id = :id",
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLLocationDao::parseSingle(RecordSet &result, Location &location,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), location, prefix);
}

bool PocoSQLLocationDao::parseSingle(Row &result, Location &location,
		const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		location = Location(LocationID::parse(result[prefix + "id"]));

	location.setName(result[prefix + "name"]);

	Place place;
	if (PocoSQLPlaceDao::parseIfIDNotNull(result, place, prefix + "place_"))
		location.setPlace(place);

	return true;
}
