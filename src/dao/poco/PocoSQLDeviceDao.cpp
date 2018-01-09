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

#include "di/Injectable.h"
#include "dao/poco/PocoSQLDeviceDao.h"
#include "dao/poco/PocoSQLLocationDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "model/Device.h"
#include "model/Gateway.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

using percent = Poco::Util::Units::Values::percent;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLDeviceDao)
BEEEON_OBJECT_CASTABLE(DeviceDao)
BEEEON_OBJECT_REF("deviceInfoProvider", &PocoSQLDeviceDao::setDeviceInfoProvider)
BEEEON_OBJECT_REF("daoManager", &PocoSQLDeviceDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLDeviceDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLDeviceDao::setQueryLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLDeviceDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLDeviceDao)

PocoSQLDeviceDao::PocoSQLDeviceDao()
{
	registerQuery(m_queryInsert);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryFetchFromGateway);
	registerQuery(m_queryFetchActiveBy);
	registerQuery(m_queryFetchInactiveBy);
	registerQuery(m_queryFetchActiveWithPrefix);
}

void PocoSQLDeviceDao::setDeviceInfoProvider(DeviceInfoProvider::Ptr provider)
{
	m_deviceInfoProvider = provider;
}

void PocoSQLDeviceDao::assertTypeValid(const Device &device)
{
	if (device.type().isNull()) {
		throw InvalidArgumentException("missing device type for: "
				+ device.id().toString());
	}

	if (m_deviceInfoProvider->findById(device.type()->id()).isNull()) {
		throw InvalidArgumentException("unrecognized device type: "
				+ device.type()->id().toString());
	}
}

bool PocoSQLDeviceDao::insert(Device &device, const Gateway &gateway)
{
	assureHasId(device);
	assureHasId(gateway);

	uint64_t id(device.id());
	uint64_t gatewayID(gateway.id());

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

	const DeviceStatus &status = device.status();

	unsigned long firstSeen = status.firstSeen().epochTime();
	unsigned long lastSeen = status.lastSeen().epochTime();

	Nullable<unsigned long> activeSince;
	if (status.active())
		activeSince = status.lastChanged().epochTime();

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

	uint64_t id(device.id());
	uint64_t gatewayID(gateway.id());

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

	const DeviceStatus &status = device.status();

	Nullable<unsigned long> activeSince;
	if (status.active())
		activeSince = status.lastChanged().epochTime();

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

	uint64_t id(device.id());
	uint64_t gatewayID(gateway.id());

	Statement sql = (session() << m_queryFetchFromGateway(),
		use(id, "id"),
		use(gatewayID, "gateway_id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, device, gateway, *m_deviceInfoProvider);
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

		uint64_t id(device.id());
		uint64_t gatewayID(device.gateway().id());

		Statement sql = (session() << m_queryFetchFromGateway(),
			use(id, "id"),
			use(gatewayID, "gateway_id")
		);

		RecordSet result = executeSelect(sql);
		if (result.rowCount() == 0) {
			it = devices.erase(it);
			continue;
		}

		if (!parseSingle(result, device, device.gateway(), *m_deviceInfoProvider)) {
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

	uint64_t gatewayID(gateway.id());

	Statement sql = (session() << m_queryFetchActiveBy(),
		use(gatewayID, "gateway_id")
	);

	RecordSet result = executeSelect(sql);
	return parseMany(result, devices, gateway, *m_deviceInfoProvider);
}

void PocoSQLDeviceDao::fetchInactiveBy(std::vector<Device> &devices, const Gateway &gateway)
{
	assureHasId(gateway);

	uint64_t gatewayID(gateway.id());

	Statement sql = (session() << m_queryFetchInactiveBy(),
		use(gatewayID, "gateway_id")
	);

	RecordSet result = executeSelect(sql);
	return parseMany(result, devices, gateway, *m_deviceInfoProvider);
}

void PocoSQLDeviceDao::fetchActiveWithPrefix(std::vector<Device> &devices,
		const Gateway &gateway,
		const DevicePrefix &prefix)
{
	assureHasId(gateway);

	uint64_t gatewayID = gateway.id();

	uint64_t minID = DeviceID(prefix, 0);
	uint64_t maxID = DeviceID(prefix, 0x00FFFFFFFFFFFFFF);

	Statement sql = (session() << m_queryFetchActiveWithPrefix(),
		use(gatewayID, "gateway_id"),
		use(minID, "min_id"),
		use(maxID, "max_id")
	);

	RecordSet result = executeSelect(sql);
	return parseMany(result, devices, gateway, *m_deviceInfoProvider);
}

bool PocoSQLDeviceDao::parseSingle(RecordSet &result, Device &device,
		const Gateway &gateway, const DeviceInfoProvider &provider,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), device, gateway, provider, prefix);
}

bool PocoSQLDeviceDao::parseSingle(Row &result, Device &device,
		const Gateway &gateway, const DeviceInfoProvider &provider,
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

	DeviceStatus status;
	status.setFirstSeen(Timestamp::fromEpochTime(result[prefix + "first_seen"]));
	status.setLastSeen(Timestamp::fromEpochTime(result[prefix + "last_seen"]));

	if (!result[prefix + "active_since"].isEmpty()) {
		status.setState(DeviceStatus::STATE_ACTIVE);
		status.setLastChanged(Timestamp::fromEpochTime(result[prefix + "active_since"]));
	}
	else {
		status.setState(DeviceStatus::STATE_INACTIVE);
		// use first-seen until the db schema is fixed
		status.setLastChanged(status.firstSeen());
	}

	device.setStatus(status);

	markLoaded(device);
	return true;
}
