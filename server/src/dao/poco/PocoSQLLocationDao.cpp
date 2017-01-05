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

PocoSQLLocationDao::PocoSQLLocationDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByIdAndPlaceId);
	registerQuery(m_queryFetchByIdAndGatewayId);
	registerQuery(m_queryFetchByPlaceId);
	registerQuery(m_queryFetchByGatewayId);
}

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
	sql << m_queryCreate(),
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
	sql << m_queryFetchById(),
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
	sql << m_queryFetchByIdAndPlaceId(),
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
	sql << m_queryFetchByIdAndGatewayId(),
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
	sql << m_queryFetchByPlaceId(),
		use(placeID, "place_id");

	execute(sql);
	RecordSet result(sql);
	parseMany(result, locations);
}

void PocoSQLLocationDao::fetchBy(Session &session,
		std::vector<Location> &locations,
		const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());

	Statement sql(session);
	sql << m_queryFetchByGatewayId(),
		use(gatewayID, "gateway_id");

	execute(sql);
	RecordSet result(sql);
	parseMany(result, locations);
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
	sql << m_queryUpdate(),
		use(name, "name"),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLLocationDao::remove(Session &session, const Location &location)
{
	assureHasId(location);
	string id(location.id().toString());

	Statement sql(session);
	sql << m_queryRemove(),
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

bool PocoSQLLocationDao::parseIfIDNotNull(Row &result, Location &location,
		const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	location = Location(LocationID::parse(id));

	if (hasColumn(result, prefix + "name"))
		location.setName(result[prefix + "name"]);

	return true;
}
