#include <cmath>

#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "dao/GatewayDao.h"
#include "dao/poco/PocoAbstractDao.h"
#include "dao/poco/PocoDaoManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

namespace BeeeOn {

class PocoSQLGatewayDao :
		public PocoAbstractDao,
		public GatewayDao {
public:
	bool insert(Gateway &gateway) override;
	bool fetch(Gateway &gateway) override;
	bool update(Gateway &gateway) override;
	bool assignAndUpdate(Gateway &gateway, const Place &place) override;
	bool assign(Gateway &gateway, const Place &place) override;
	bool unassign(Gateway &gateway) override;
	bool fetchFromPlace(Gateway &gateway, const Place &place) override;
	void fetchAccessible(
			std::vector<Gateway> &gateways,
			const User &user) override;

protected:
	bool insert(Session &session, Gateway &gateway);
	bool fetch(Session &session, Gateway &gateway);
	bool update(Session &session, Gateway &gateway);
	bool assignAndUpdate(Session &session,
			Gateway &gateway, const Place &place);
	bool assign(Session &session,
			Gateway &gateway, const Place &place);
	bool unassign(Session &session, Gateway &gateway);
	bool fetchFromPlace(Session &session,
			Gateway &gateway, const Place &place);
	void fetchAccessible(Session &session,
			std::vector<Gateway> &gateways,
			const User &user);
};

}

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
	sql << "INSERT INTO gateways ("
		" id,"
		" name,"
		" place_id,"
		" altitude,"
		" latitude,"
		" longitude"
		")"
		" VALUES ("
		" :id,"
		" :name,"
		" :place_id,"
		" :altitude,"
		" :latitude,"
		" :longitude"
		")",
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
	string name;
	Nullable<string> placeID;
	Nullable<string> placeName;
	Nullable<double> altitude;
	Nullable<double> latitude;
	Nullable<double> longitude;

	Statement sql(session);
	sql << "SELECT"
		" g.name,"
		" g.altitude,"
		" g.latitude,"
		" g.longitude,"
		" p.id,"
		" p.name"
		" FROM gateways AS g"
		" LEFT JOIN places AS p ON g.place_id = p.id"
		" WHERE g.id = :id",
		use(id, "id"),
		into(name),
		into(altitude),
		into(latitude),
		into(longitude),
		into(placeID),
		into(placeName);

	if (execute(sql) == 0)
		return false;

	if (!placeID.isNull()) {
		Place place(PlaceID::parse(placeID));
		place.setName(placeName);

		gateway.setPlace(place);
	}

	gateway.setName(name);
	gateway.setAltitude(altitude.isNull()? NAN : altitude.value());
	gateway.setLatitude(latitude.isNull()? NAN : latitude.value());
	gateway.setLongitude(longitude.isNull()? NAN : longitude.value());

	return true;
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
	sql << "UPDATE gateways"
		" SET"
		" name = :name,"
		" altitude = :altitude,"
		" latitude = :latitude,"
		" longitude = :longitude"
		" WHERE id = :id",
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
	sql << "UPDATE gateways"
		" SET"
		" name = :name,"
		" altitude = :altitude,"
		" latitude = :latitude,"
		" longitude = :longitude,"
		" place_id = :place_id"
		" WHERE id = :id",
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
	sql << "UPDATE gateways"
		" SET"
		" place_id = :place_id"
		" WHERE id = :id",
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
	sql << "UPDATE gateways"
		" SET"
		" place_id = NULL"
		" WHERE id = :id",
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
	string name;
	Nullable<double> altitude;
	Nullable<double> latitude;
	Nullable<double> longitude;
	string placeName;

	Statement sql(session);
	sql << "SELECT"
		" g.name,"
		" g.altitude,"
		" g.latitude,"
		" g.longitude,"
		" p.name"
		" FROM gateways AS g"
		" JOIN places AS p ON g.place_id = p.id"
		" WHERE g.id = :id AND g.place_id = :place_id",
		use(id, "id"),
		use(placeID, "place_id"),
		into(name),
		into(altitude),
		into(latitude),
		into(longitude),
		into(placeName);

	if (execute(sql) == 0)
		return false;

	Place freshPlace(PlaceID::parse(placeID));
	freshPlace.setName(placeName);
	gateway.setPlace(place);

	gateway.setName(name);
	gateway.setAltitude(altitude.isNull()? NAN : altitude.value());
	gateway.setLatitude(latitude.isNull()? NAN : latitude.value());
	gateway.setLongitude(longitude.isNull()? NAN : longitude.value());

	return true;
}

void PocoSQLGatewayDao::fetchAccessible(Session &session,
		std::vector<Gateway> &gateways,
		const User &user)
{
	assureHasId(user);

	string userID(user.id().toString());

	Statement sql(session);
	sql << "SELECT"
		" DISTINCT g.id,"
		" g.name,"
		" g.altitude,"
		" g.latitude,"
		" g.longitude,"
		" p.id,"
		" p.name"
		" FROM gateways AS g"
		" JOIN places AS p ON g.place_id = p.id"
		" JOIN roles_in_place AS r ON g.place_id = r.place_id"
		" JOIN verified_identities AS v"
		"   ON v.identity_id = r.identity_id"
		" WHERE v.user_id = :user_id",
		use(userID, "user_id");

	execute(sql);
	RecordSet result(sql);

	for (auto row : result) {
		Place place(PlaceID::parse(row.get(5)));
		place.setName(row.get(6));

		Gateway gateway(GatewayID::parse(row.get(0)));
		gateway.setName(row.get(1));
		gateway.setAltitude(nanWhenEmpty(row.get(2)));
		gateway.setLatitude(nanWhenEmpty(row.get(3)));
		gateway.setLongitude(nanWhenEmpty(row.get(4)));
		gateway.setPlace(place);

		gateways.push_back(gateway);
	}
}
