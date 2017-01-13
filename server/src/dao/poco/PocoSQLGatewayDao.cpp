#include <cmath>

#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "dao/poco/PocoSQLGatewayDao.h"
#include "dao/poco/PocoSQLPlaceDao.h"
#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Net;
using namespace BeeeOn;

BEEEON_OBJECT(PocoSQLGatewayDao, BeeeOn::PocoSQLGatewayDao)

PocoSQLGatewayDao::PocoSQLGatewayDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryAssignAndUpdate);
	registerQuery(m_queryAssign);
	registerQuery(m_queryUnassign);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByPlaceId);
	registerQuery(m_queryFetchAccessible);
	registerQuery(m_queryLegacyFetchById);
	registerQuery(m_queryLegacyFetchAccessible);
}

bool PocoSQLGatewayDao::insert(Gateway &gateway)
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

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(name, "name"),
		use(placeID, "place_id"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude")
	);

	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::fetch(Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());

	Statement sql = (session() << m_queryFetchById(),
		use(id, "id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, gateway);
}

bool PocoSQLGatewayDao::fetch(LegacyGateway &gateway, const User &user)
{
	assureHasId(gateway);
	assureHasId(user);

	string id(gateway.id().toString());
	string userID(user.id().toString());

	Statement sql = (session() << m_queryLegacyFetchById(),
		use(userID, "user_id"),
		use(id, "id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, gateway);
}

bool PocoSQLGatewayDao::update(Gateway &gateway)
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

	Statement sql = (session() << m_queryUpdate(),
		use(name, "name"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude"),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::assignAndUpdate(Gateway &gateway, const Place &place)
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

	Statement sql = (session() << m_queryAssignAndUpdate(),
		use(name, "name"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude"),
		use(placeID, "place_id"),
		use(id, "id")
	);

	gateway.setPlace(place);
	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::assign(Gateway &gateway, const Place &place)
{
	assureHasId(gateway);
	assureHasId(place);

	string id(gateway.id().toString());
	string placeID(place.id().toString());

	Statement sql = (session() << m_queryAssign(),
		use(placeID, "place_id"),
		use(id, "id")
	);

	gateway.setPlace(place);
	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::unassign(Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());

	Statement sql = (session() << m_queryUnassign(),
		use(id, "id")
	);

	return execute(sql) > 0;
}

bool PocoSQLGatewayDao::fetchFromPlace(Gateway &gateway, const Place &place)
{
	assureHasId(gateway);
	assureHasId(place);

	string id(gateway.id().toString());
	string placeID(place.id().toString());

	Statement sql = (session() << m_queryFetchByPlaceId(),
		use(id, "id"),
		use(placeID, "place_id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, gateway);
}

void PocoSQLGatewayDao::fetchAccessible(std::vector<Gateway> &gateways,
		const User &user)
{
	assureHasId(user);

	string userID(user.id().toString());

	Statement sql = (session() << m_queryFetchAccessible(),
		use(userID, "user_id")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany<Gateway>(result, gateways);
}

void PocoSQLGatewayDao::fetchAccessible(std::vector<LegacyGateway> &gateways,
		const User &user)
{
	assureHasId(user);

	string userID(user.id().toString());

	Statement sql = (session() << m_queryLegacyFetchAccessible(),
		use(userID, "user_id")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany<LegacyGateway>(result, gateways);
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
	gateway.setVersion(emptyWhenNull(result[prefix + "version"]));
	gateway.setIPAddress(whenNull(result[prefix + "ip"], string("0.0.0.0")));

	Nullable<Poco::DateTime> lastChanged;
	if (!result[prefix + "last_changed"].isEmpty())
		lastChanged = Timestamp::fromEpochTime(result[prefix + "last_changed"]);

	gateway.setLastChanged(lastChanged);

	Place place;
	if (PocoSQLPlaceDao::parseIfIDNotNull(result, place, prefix + "place_"))
		gateway.setPlace(place);

	return true;
}

bool PocoSQLGatewayDao::parseSingle(Row &result, LegacyGateway &gateway,
		const string &prefix)
{
	if (!parseSingle(result, static_cast<Gateway &>(gateway)))
		return false;

	Poco::Dynamic::Var &accessLevel = result[prefix + "access_level"];
	if (!accessLevel.isEmpty())
		gateway.setAccessLevel(AccessLevel(accessLevel.convert<unsigned int>()));

	User owner;
	if (PocoSQLUserDao::parseIfIDNotNull(result, owner, prefix + "owner_"))
		gateway.setOwner(owner);

	gateway.setUserCount(result[prefix + "roles_count"].convert<unsigned int>());
	gateway.setDeviceCount(result[prefix + "devices_count"].convert<unsigned int>());

	return true;
}
