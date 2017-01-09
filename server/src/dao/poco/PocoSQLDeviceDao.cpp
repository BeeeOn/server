#include <cmath>

#include <Poco/Exception.h>
#include <Poco/DateTime.h>
#include <Poco/DateTimeParser.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "dao/poco/PocoSQLDeviceDao.h"
#include "dao/poco/PocoSQLLocationDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "model/Device.h"
#include "model/Gateway.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

using percent = Poco::Util::Units::Values::percent;

BEEEON_OBJECT(PocoSQLDeviceDao, BeeeOn::PocoSQLDeviceDao)

PocoSQLDeviceDao::PocoSQLDeviceDao()
{
	registerQuery(m_queryInsert);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryFetchFromGateway);
	registerQuery(m_queryFetchActiveBy);
	registerQuery(m_queryFetchInactiveBy);
}

bool PocoSQLDeviceDao::insert(Device &device, const Gateway &gateway)
{
	assureHasId(device);
	assureHasId(gateway);

	string id(device.id().toString());
	string gatewayID(gateway.id().toString());

	Nullable<string> locationID;
	if (!device.location().id().isNull())
		locationID = device.location().id().toString();

	string name = device.name();
	unsigned int type = device.type();
	unsigned int refresh = device.refresh().totalSeconds();

	Nullable<unsigned int> battery;
	if (!device.battery().isNull())
		battery = device.battery().value().get();

	Nullable<unsigned int> signal;
	if (!device.signal().isNull())
		signal = device.signal().value().get();

	unsigned long firstSeen = device.firstSeen().timestamp().epochTime();
	unsigned long lastSeen = device.lastSeen().timestamp().epochTime();

	Nullable<unsigned long> activeSince;
	if (!device.activeSince().isNull())
		activeSince = device.activeSince().value().timestamp().epochTime();

	Statement sql = (session() << m_queryInsert(),
		use(id, "id"),
		use(gatewayID, "gateway_id"),
		use(locationID, "location_id"),
		use(name, "name"),
		use(type, "type"),
		use(refresh, "refresh"),
		use(battery, "battery"),
		use(signal, "signal"),
		use(firstSeen, "first_seen"),
		use(lastSeen, "last_seen"),
		use(activeSince, "active_since")
	);

	return execute(sql) > 0;
}

bool PocoSQLDeviceDao::update(Device &device, const Gateway &gateway)
{
	assureHasId(device);
	assureHasId(gateway);

	string id(device.id().toString());
	string gatewayID(gateway.id().toString());

	Nullable<string> locationID;
	if (!device.location().id().isNull())
		locationID = device.location().id().toString();

	string name = device.name();
	unsigned int type = device.type();
	unsigned int refresh = device.refresh().totalSeconds();

	Nullable<unsigned int> battery;
	if (!device.battery().isNull())
		battery = device.battery().value().get();

	Nullable<unsigned int> signal;
	if (!device.signal().isNull())
		signal = device.signal().value().get();

	Nullable<unsigned long> activeSince;
	if (!device.activeSince().isNull())
		activeSince = device.activeSince().value().timestamp().epochTime();

	Statement sql = (session() << m_queryUpdate(),
		use(locationID, "location_id"),
		use(name, "name"),
		use(type, "type"),
		use(refresh, "refresh"),
		use(battery, "battery"),
		use(signal, "signal"),
		use(activeSince, "active_since"),
		use(id, "id"),
		use(gatewayID, "gateway_id")
	);

	return execute(sql) > 0;
}

bool PocoSQLDeviceDao::fetch(Device &device, const Gateway &gateway)
{
	assureHasId(device);
	assureHasId(gateway);

	string id(device.id().toString());
	string gatewayID(gateway.id().toString());

	Statement sql = (session() << m_queryFetchFromGateway(),
		use(id, "id"),
		use(gatewayID, "gateway_id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, device, gateway);
}

void PocoSQLDeviceDao::fetchActiveBy(std::vector<Device> &devices,
		const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());

	Statement sql = (session() << m_queryFetchActiveBy(),
		use(gatewayID, "gateway_id")
	);

	execute(sql);
	RecordSet result(sql);
	return parseMany(result, devices, gateway);
}

void PocoSQLDeviceDao::fetchInactiveBy(std::vector<Device> &devices, const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());

	Statement sql = (session() << m_queryFetchInactiveBy(),
		use(gatewayID, "gateway_id")
	);

	execute(sql);
	RecordSet result(sql);
	return parseMany(result, devices, gateway);
}

bool PocoSQLDeviceDao::parseSingle(RecordSet &result, Device &device,
		const Gateway &gateway, const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), device, gateway, prefix);
}

bool PocoSQLDeviceDao::parseSingle(Row &result, Device &device,
		const Gateway &gateway, const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		device = Device(DeviceID::parse(result[prefix + "id"]));

	device.setGateway(gateway); // device is always associated to a gateway

	Location location;
	if (PocoSQLLocationDao::parseIfIDNotNull(result, location, prefix + "location_"))
		device.setLocation(location);

	device.setName(result[prefix + "name"]);
	device.setType(result[prefix + "type"]);
	device.setRefresh(result[prefix + "refresh"].convert<unsigned int>());
	device.setBattery(whenNull(result[prefix + "battery"], 0));
	device.setSignal(whenNull(result[prefix + "signal"], 0));
	device.setFirstSeen(Timestamp::fromEpochTime(result[prefix + "first_seen"]));
	device.setLastSeen(Timestamp::fromEpochTime(result[prefix + "last_seen"]));

	Nullable<DateTime> activeSince;
	if (!result[prefix + "active_since"].isEmpty())
		activeSince = Timestamp::fromEpochTime(result[prefix + "active_since"]);

	device.setActiveSince(activeSince);

	return true;
}
