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

PocoSQLDeviceDao::PocoSQLDeviceDao():
	m_infoProvider(&NullInfoProvider<DeviceInfo>::instance())
{
	injector<PocoSQLDeviceDao, InfoProvider<DeviceInfo>>(
		"deviceInfoProvider",
		&PocoSQLDeviceDao::setDeviceInfoProvider
	);

	registerQuery(m_queryInsert);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryFetchFromGateway);
	registerQuery(m_queryFetchActiveBy);
	registerQuery(m_queryFetchInactiveBy);
}

void PocoSQLDeviceDao::setDeviceInfoProvider(InfoProvider<DeviceInfo> *provider)
{
	m_infoProvider = provider == NULL?
		&NullInfoProvider<DeviceInfo>::instance() : provider;
}

void PocoSQLDeviceDao::assertTypeValid(const Device &device)
{
	if (device.type().isNull()) {
		throw InvalidArgumentException("missing device type for: "
				+ device.id().toString());
	}

	if (m_infoProvider->findById(device.type()->id()).isNull()) {
		throw InvalidArgumentException("unrecognized device type: "
				+ device.type()->id().toString());
	}
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

	assertTypeValid(device);

	unsigned int type = device.type()->id();
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

	assertTypeValid(device);

	unsigned int type = device.type()->id();
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
	return parseSingle(result, device, gateway, *m_infoProvider);
}

void PocoSQLDeviceDao::fetchMany(std::list<Device> &devices)
{
	auto it = devices.begin();

	while (it != devices.end()) {
		Device &device = *it;

		if (!device.hasId() || !device.gateway().hasId()) {
			it = devices.erase(it);
			continue;
		}

		string id(device.id().toString());
		string gatewayID(device.gateway().id().toString());

		Statement sql = (session() << m_queryFetchFromGateway(),
			use(id, "id"),
			use(gatewayID, "gateway_id")
		);

		if (execute(sql) == 0) {
			it = devices.erase(it);
			continue;
		}

		RecordSet result(sql);
		if (!parseSingle(result, device, device.gateway(), *m_infoProvider)) {
			it = devices.erase(it);
			continue;
		}

		++it;
	}

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
	return parseMany(result, devices, gateway, *m_infoProvider);
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
	return parseMany(result, devices, gateway, *m_infoProvider);
}

bool PocoSQLDeviceDao::parseSingle(RecordSet &result, Device &device,
		const Gateway &gateway, const InfoProvider<DeviceInfo> &provider,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), device, gateway, provider, prefix);
}

bool PocoSQLDeviceDao::parseSingle(Row &result, Device &device,
		const Gateway &gateway, const InfoProvider<DeviceInfo> &provider,
		const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		device.setId(DeviceID::parse(result[prefix + "id"]));

	device.setGateway(gateway); // device is always associated to a gateway

	Location location;
	if (PocoSQLLocationDao::parseIfIDNotNull(result, location, prefix + "location_"))
		device.setLocation(location);

	device.setName(result[prefix + "name"]);
	device.setType(provider.findById(DeviceInfoID::parse(result[prefix + "type"])));
	device.setRefresh(result[prefix + "refresh"].convert<unsigned int>());
	device.setBattery(whenNull(result[prefix + "battery"], 0));
	device.setSignal(whenNull(result[prefix + "signal"], 0));
	device.setFirstSeen(Timestamp::fromEpochTime(result[prefix + "first_seen"]));
	device.setLastSeen(Timestamp::fromEpochTime(result[prefix + "last_seen"]));

	Nullable<DateTime> activeSince;
	if (!result[prefix + "active_since"].isEmpty())
		activeSince = Timestamp::fromEpochTime(result[prefix + "active_since"]);

	device.setActiveSince(activeSince);

	markLoaded(device);
	return true;
}
