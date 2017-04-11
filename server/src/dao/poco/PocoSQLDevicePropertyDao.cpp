#include <cmath>

#include <Poco/Exception.h>
#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "di/Injectable.h"
#include "dao/TransactionManager.h"
#include "dao/poco/PocoDaoManager.h"
#include "dao/poco/PocoSQLDevicePropertyDao.h"
#include "model/Device.h"
#include "model/DeviceProperty.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLDevicePropertyDao)
BEEEON_OBJECT_CASTABLE(DevicePropertyDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLDevicePropertyDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLDevicePropertyDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLDevicePropertyDao::setSQLLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLDevicePropertyDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLDevicePropertyDao)

PocoSQLDevicePropertyDao::PocoSQLDevicePropertyDao()
{
	registerQuery(m_queryInsert);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetch);
	registerQuery(m_queryFetchByDevice);
}

void PocoSQLDevicePropertyDao::assureKeyValid(const DeviceProperty &property) const
{
	if (property.key() == DevicePropertyKey::KEY_INVALID)
		throw InvalidArgumentException("invalid key for device property");
}

bool PocoSQLDevicePropertyDao::insert(DeviceProperty &property, const Device &device)
{
	assureKeyValid(property);
	assureHasId(device);
	assureHasId(device.gateway());

	uint64_t key(property.key().raw());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	string value = property.value();
	Nullable<string> params;

	if (!property.params().empty())
		params = property.params().toString();

	Statement sql = (session() << m_queryInsert(),
		use(deviceID, "device_id"),
		use(gatewayID, "gateway_id"),
		use(key, "key"),
		use(value, "value"),
		use(params, "params")
	);

	return execute(sql) > 0;
}

bool PocoSQLDevicePropertyDao::update(DeviceProperty &property, const Device &device)
{
	assureKeyValid(property);
	assureHasId(device);
	assureHasId(device.gateway());

	uint64_t key(property.key().raw());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	string value = property.value();
	Nullable<string> params;

	if (!property.params().empty())
		params = property.params().toString();

	Statement sql = (session() << m_queryUpdate(),
		use(value, "value"),
		use(params, "params"),
		use(deviceID, "device_id"),
		use(gatewayID, "gateway_id"),
		use(key, "key")
	);

	return execute(sql) > 0;
}

bool PocoSQLDevicePropertyDao::remove(const DeviceProperty &property, const Device &device)
{
	assureKeyValid(property);
	assureHasId(device);
	assureHasId(device.gateway());

	uint64_t key(property.key().raw());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	Statement sql = (session() << m_queryRemove(),
		use(deviceID, "device_id"),
		use(gatewayID, "gateway_id"),
		use(key, "key")
	);

	return execute(sql) > 0;
}

bool PocoSQLDevicePropertyDao::fetch(DeviceProperty &property, const Device &device)
{
	assureKeyValid(property);
	assureHasId(device);
	assureHasId(device.gateway());

	uint64_t key(property.key().raw());
	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	Statement sql = (session() << m_queryFetch(),
		use(deviceID, "device_id"),
		use(gatewayID, "gateway_id"),
		use(key, "key")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, property);
}

void PocoSQLDevicePropertyDao::fetchByDevice(std::list<DeviceProperty> &properties, const Device &device)
{
	assureHasId(device);
	assureHasId(device.gateway());

	uint64_t deviceID(device.id());
	uint64_t gatewayID(device.gateway().id());

	Statement sql = (session() << m_queryFetchByDevice(),
		use(deviceID, "device_id"),
		use(gatewayID, "gateway_id")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany(result, properties);
}


bool PocoSQLDevicePropertyDao::parseSingle(RecordSet &result,
		DeviceProperty &property,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), property, prefix);
}

bool PocoSQLDevicePropertyDao::parseSingle(Row &result,
		DeviceProperty &property,
		const string &prefix)
{
	if (hasColumn(result, prefix + "key")) {
		property.setKey(DevicePropertyKey::fromRaw(
			result[prefix + "key"].convert<int>()
		));
	}

	property.setValue(result[prefix + "value"]);

	if (result[prefix + "params"].isEmpty())
		property.setParams(CryptoParams::createEmpty());
	else
		property.setParams(CryptoParams::parse(result[prefix + "params"]));

	//markLoaded(property)
	return true;
}
