#include <Poco/Exception.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLLocationDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLLocationDao)
BEEEON_OBJECT_CASTABLE(LocationDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLLocationDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLLocationDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLLocationDao::setSQLLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLLocationDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLLocationDao)

PocoSQLLocationDao::PocoSQLLocationDao()
{
	registerQuery(m_queryCreate);
	registerQuery(m_queryUpdate);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetchById);
	registerQuery(m_queryFetchByIdAndGatewayId);
	registerQuery(m_queryFetchByGatewayId);
}

void PocoSQLLocationDao::create(Location &location)
{
	location.setId(LocationID::random());
	string id(location.id().toString());
	string name(location.name());
	string gatewayID(location.gateway().id().toString());

	Statement sql = (session() << m_queryCreate(),
		use(id, "id"),
		use(name, "name"),
		use(gatewayID, "gateway_id")
	);

	execute(sql);
}

bool PocoSQLLocationDao::fetch(Location &location)
{
	assureHasId(location);
	string id(location.id().toString());

	Statement sql = (session() << m_queryFetchById(),
		use(id, "id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, location);
}

bool PocoSQLLocationDao::fetchFrom(Location &location, const Gateway &gateway)
{
	assureHasId(location);
	assureHasId(gateway);

	string id(location.id().toString());
	string gatewayID(gateway.id().toString());

	Statement sql = (session() << m_queryFetchByIdAndGatewayId(),
		use(id, "id"),
		use(gatewayID, "gateway_id")
	);

	if (execute(sql) == 0)
		return false;

	RecordSet result(sql);
	return parseSingle(result, location);
}

void PocoSQLLocationDao::fetchBy(std::vector<Location> &locations,
		const Gateway &gateway)
{
	assureHasId(gateway);

	string gatewayID(gateway.id().toString());

	Statement sql = (session() << m_queryFetchByGatewayId(),
		use(gatewayID, "gateway_id")
	);

	execute(sql);
	RecordSet result(sql);
	parseMany(result, locations);
}

bool PocoSQLLocationDao::update(Location &location)
{
	assureHasId(location);
	assureHasId(location.gateway());

	string id(location.id().toString());
	string name(location.name());

	Statement sql = (session() << m_queryUpdate(),
		use(id, "id"),
		use(name, "name")
	);

	return execute(sql) > 0;
}

bool PocoSQLLocationDao::remove(const Location &location)
{
	assureHasId(location);
	string id(location.id().toString());

	Statement sql = (session() << m_queryRemove(),
		use(id, "id")
	);

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
		location.setId(LocationID::parse(result[prefix + "id"]));

	location.setName(result[prefix + "name"]);

	Gateway gateway(GatewayID::parse(result[prefix + "gateway_id"]));
	location.setGateway(gateway);

	markLoaded(location);
	return true;
}

bool PocoSQLLocationDao::parseIfIDNotNull(Row &result, Location &location,
		const string &prefix)
{
	const string id = emptyWhenNull(result[prefix + "id"]);
	if (id.empty())
		return false;

	location.setId(LocationID::parse(id));

	if (hasColumn(result, prefix + "name"))
		location.setName(result[prefix + "name"]);

	markLoaded(location);
	return true;
}
