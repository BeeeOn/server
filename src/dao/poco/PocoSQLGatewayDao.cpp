#include <cmath>

#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLGatewayDao.h"
#include "dao/poco/PocoSQLUserDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "l10n/TimeZone.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Net;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLGatewayDao)
BEEEON_OBJECT_CASTABLE(GatewayDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLGatewayDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLGatewayDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLGatewayDao::setQueryLoader)
BEEEON_OBJECT_REF("timeZoneProvider", &PocoSQLGatewayDao::setTimeZoneProvider)
BEEEON_OBJECT_HOOK("done", &PocoSQLGatewayDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLGatewayDao)

PocoSQLGatewayDao::PocoSQLGatewayDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchAccessible);
	registerQuery(m_queryLegacyFetchById);
	registerQuery(m_queryLegacyFetchAccessible);
}

void PocoSQLGatewayDao::setTimeZoneProvider(TimeZoneProvider::Ptr provider)
{
	m_timeZoneProvider = provider;
}

bool PocoSQLGatewayDao::insert(Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());
	string name(gateway.name());

	Nullable<int> altitude = gateway.altitude();

	Nullable<double> latitude;
	if (!std::isnan(gateway.latitude()))
		latitude = gateway.latitude();

	Nullable<double> longitude;
	if (!std::isnan(gateway.longitude()))
		longitude = gateway.longitude();

	string zone = gateway.timeZone().id();

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(name, "name"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude"),
		use(zone, "timezone")
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

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, m_timeZoneProvider, gateway);
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

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, m_timeZoneProvider, gateway);
}

bool PocoSQLGatewayDao::update(Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());
	string name(gateway.name());

	Nullable<int> altitude = gateway.altitude();

	Nullable<double> latitude;
	if (!std::isnan(gateway.latitude()))
		latitude = gateway.latitude();

	Nullable<double> longitude;
	if (!std::isnan(gateway.longitude()))
		longitude = gateway.longitude();

	string zone = gateway.timeZone().id();

	Statement sql = (session() << m_queryUpdate(),
		use(id, "id"),
		use(name, "name"),
		use(altitude, "altitude"),
		use(latitude, "latitude"),
		use(longitude, "longitude"),
		use(zone, "timezone")
	);

	return execute(sql) > 0;
}

void PocoSQLGatewayDao::fetchAccessible(std::vector<Gateway> &gateways,
		const User &user)
{
	assureHasId(user);

	string userID(user.id().toString());

	Statement sql = (session() << m_queryFetchAccessible(),
		use(userID, "user_id")
	);

	RecordSet result = executeSelect(sql);
	parseMany<Gateway>(result, m_timeZoneProvider, gateways);
}

void PocoSQLGatewayDao::fetchAccessible(std::vector<LegacyGateway> &gateways,
		const User &user)
{
	assureHasId(user);

	string userID(user.id().toString());

	Statement sql = (session() << m_queryLegacyFetchAccessible(),
		use(userID, "user_id")
	);

	RecordSet result = executeSelect(sql);
	parseMany<LegacyGateway>(result, m_timeZoneProvider, gateways);
}

bool PocoSQLGatewayDao::parseSingle(Row &result,
		TimeZoneProvider::Ptr provider,
		Gateway &gateway,
		const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		gateway.setId(GatewayID::parse(result[prefix + "id"]));

	gateway.setName(emptyWhenNull(result[prefix + "name"]));
	gateway.setAltitude(nullable<int>(result[prefix + "altitude"]));
	gateway.setLatitude(nanWhenEmpty(result[prefix + "latitude"]));

	Nullable<TimeZone> zone = provider->findById(result[prefix + "timezone"]);
	if (!zone.isNull())
		gateway.setTimeZone(zone);
	else
		gateway.setTimeZone(TimeZone::system());

	gateway.setLongitude(nanWhenEmpty(result[prefix + "longitude"]));
	gateway.setVersion(emptyWhenNull(result[prefix + "version"]));
	gateway.setIPAddress(whenNull(result[prefix + "ip"], string("0.0.0.0")));

	Nullable<Poco::DateTime> lastChanged;
	if (!result[prefix + "last_changed"].isEmpty())
		lastChanged = Timestamp::fromEpochTime(result[prefix + "last_changed"]);

	gateway.setLastChanged(lastChanged);

	markLoaded(gateway);
	return true;
}

bool PocoSQLGatewayDao::parseSingle(Row &result,
		TimeZoneProvider::Ptr provider,
		LegacyGateway &gateway,
		const string &prefix)
{
	if (!parseSingle(result, provider, static_cast<Gateway &>(gateway)))
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
