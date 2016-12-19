#include <cmath>

#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "dao/poco/PocoSQLGatewayDao.h"
#include "dao/poco/PocoSQLPlaceDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLGatewayDao, BeeeOn::PocoSQLGatewayDao)

bool PocoSQLGatewayDao::insert(Gateway &gateway)
{
	Session session(manager().pool().get());
	return insert(session, gateway);
}

bool PocoSQLGatewayDao::fetch(Gateway &gateway)
{
	Session session(manager().pool().get());
	return fetch(session, gateway);
}

bool PocoSQLGatewayDao::update(Gateway &gateway)
{
	Session session(manager().pool().get());
	return update(session, gateway);
}

bool PocoSQLGatewayDao::assign(Gateway &gateway, const Place &place)
{
	Session session(manager().pool().get());
	return assign(session, gateway, place);
}

bool PocoSQLGatewayDao::assignAndUpdate(Gateway &gateway, const Place &place)
{
	Session session(manager().pool().get());
	return assignAndUpdate(session, gateway, place);
}

bool PocoSQLGatewayDao::unassign(Gateway &gateway)
{
	Session session(manager().pool().get());
	return unassign(session, gateway);
}

bool PocoSQLGatewayDao::fetchFromPlace(
		Gateway &gateway,
		const Place &place)
{
	Session session(manager().pool().get());
	return fetchFromPlace(session, gateway, place);
}

void PocoSQLGatewayDao::fetchAccessible(
		std::vector<Gateway> &gateways,
		const User &user)
{
	Session session(manager().pool().get());
	return fetchAccessible(session, gateways, user);
}

bool PocoSQLGatewayDao::insert(Session &session, Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());
	string name(gateway.name());
	Nullable<string> placeID;

	if (gateway.hasPlace())
		placeID = gateway.place().id().toString();

	Nullable<double> altitude;
	if (!std::isnan(gateway.altitude()))
		altitude = gateway.altitude();

	Nullable<double> latitude;
	if (!std::isnan(gateway.latitude()))
		latitude = gateway.latitude();

	Nullable<double> longitude;
	if (!std::isnan(gateway.longitude()))
		longitude = gateway.longitude();

	Statement sql(session);
	sql << findQuery("gateways.create"),
		use(id, "id"),
		use(name, "name"),
		use(placeID, "place_id"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude");

	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::fetch(Session &session, Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());

	Statement sql(session);
	sql << findQuery("gateways.fetch.by.id"),
		use(id, "id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, gateway);
}

bool PocoSQLGatewayDao::update(Session &session, Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());
	string name(gateway.name());

	Nullable<double> altitude;
	if (!std::isnan(gateway.altitude()))
		altitude = gateway.altitude();

	Nullable<double> latitude;
	if (!std::isnan(gateway.latitude()))
		latitude = gateway.latitude();

	Nullable<double> longitude;
	if (!std::isnan(gateway.longitude()))
		longitude = gateway.longitude();

	Statement sql(session);
	sql << findQuery("gateways.update"),
		use(name, "name"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude"),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::assignAndUpdate(Session &session,
		Gateway &gateway, const Place &place)
{
	assureHasId(gateway);
	assureHasId(place);

	string id(gateway.id().toString());
	string name(gateway.name());
	string placeID(place.id().toString());

	Nullable<double> altitude;
	if (!std::isnan(gateway.altitude()))
		altitude = gateway.altitude();

	Nullable<double> latitude;
	if (!std::isnan(gateway.latitude()))
		latitude = gateway.latitude();

	Nullable<double> longitude;
	if (!std::isnan(gateway.longitude()))
		longitude = gateway.longitude();

	Statement sql(session);
	sql << findQuery("gateways.assign.and.update"),
		use(name, "name"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude"),
		use(placeID, "place_id"),
		use(id, "id");

	gateway.setPlace(place);
	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::assign(Session &session,
		Gateway &gateway, const Place &place)
{
	assureHasId(gateway);
	assureHasId(place);

	string id(gateway.id().toString());
	string placeID(place.id().toString());

	Statement sql(session);
	sql << findQuery("gateways.assign"),
		use(placeID, "place_id"),
		use(id, "id");

	gateway.setPlace(place);
	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::unassign(Session &session, Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());

	Statement sql(session);
	sql << findQuery("gateways.unassign"),
		use(id, "id");

	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::fetchFromPlace(Session &session,
		Gateway &gateway, const Place &place)
{
	assureHasId(gateway);
	assureHasId(place);

	string id(gateway.id().toString());
	string placeID(place.id().toString());

	Statement sql(session);
	sql << findQuery("gateways.fetch.by.place_id"),
		use(id, "id"),
		use(placeID, "place_id");

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, gateway);
}

void PocoSQLGatewayDao::fetchAccessible(Session &session,
		std::vector<Gateway> &gateways,
		const User &user)
{
	assureHasId(user);

	string userID(user.id().toString());

	Statement sql(session);
	sql << findQuery("gateways.fetch.accessible"),
		use(userID, "user_id");

	execute(sql);
	RecordSet result(sql);
	parseMany(result, gateways);
}

bool PocoSQLGatewayDao::parseSingle(RecordSet &result, Gateway &gateway,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), gateway, prefix);
}

bool PocoSQLGatewayDao::parseSingle(Row &result, Gateway &gateway,
		const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		gateway = Gateway(GatewayID::parse(result[prefix + "id"]));

	gateway.setName(emptyWhenNull(result[prefix + "name"]));
	gateway.setAltitude(nanWhenEmpty(result[prefix + "altitude"]));
	gateway.setLatitude(nanWhenEmpty(result[prefix + "latitude"]));
	gateway.setLongitude(nanWhenEmpty(result[prefix + "longitude"]));

	Place place;
	if (PocoSQLPlaceDao::parseIfIDNotNull(result, place, prefix + "place_"))
		gateway.setPlace(place);

	return true;
}
