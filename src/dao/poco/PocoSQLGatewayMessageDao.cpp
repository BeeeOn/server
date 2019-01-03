#include <Poco/Data/Session.h>
#include <Poco/Data/SessionPool.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>
#include <Poco/Dynamic/Var.h>

#include "di/Injectable.h"
#include "util/JsonUtil.h"
#include "dao/poco/PocoDaoManager.h"
#include "dao/poco/PocoSQLGatewayMessageDao.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLGatewayMessageDao)
BEEEON_OBJECT_CASTABLE(GatewayMessageDao)
BEEEON_OBJECT_PROPERTY("daoManager", &PocoSQLGatewayMessageDao::setDaoManager)
BEEEON_OBJECT_PROPERTY("transactionManager", &PocoSQLGatewayMessageDao::setTransactionManager)
BEEEON_OBJECT_PROPERTY("sqlLoader", &PocoSQLGatewayMessageDao::setQueryLoader)
BEEEON_OBJECT_PROPERTY("profiler", &PocoSQLGatewayMessageDao::setQueryProfiler)
BEEEON_OBJECT_HOOK("done", &PocoSQLGatewayMessageDao::loadQueries)
BEEEON_OBJECT_HOOK("cleanup", &PocoSQLGatewayMessageDao::clearQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLGatewayMessageDao)

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace Poco::Dynamic;
using namespace BeeeOn;

PocoSQLGatewayMessageDao::PocoSQLGatewayMessageDao()
{
	registerQuery(m_queryInsert);
	registerQuery(m_queryRemove);
	registerQuery(m_queryFetch);
	registerQuery(m_queryFetchByGateway);
}

bool PocoSQLGatewayMessageDao::insert(GatewayMessage &message, size_t limit)
{
	message.setId(GatewayMessageID::random());

	string id = message.id().toString();
	string gatewayID = message.gateway().id().toString();
	uint64_t at = message.at().epochMicroseconds();
	int severity = static_cast<int>(message.severity().raw());
	string key = message.key();
	string context = Var::toString(message.context());

	Statement sql = (session() << m_queryInsert(),
		use(id, "id"),
		use(gatewayID, "gateway_id"),
		use(at, "at"),
		use(severity, "severity"),
		use(key, "key"),
		use(context, "context"),
		use(limit, "limit")
	);

	return execute(sql) > 0;
}

bool PocoSQLGatewayMessageDao::remove(
		const GatewayMessage &message,
		const Gateway &gateway)
{
	string id = message.id().toString();
	string gatewayID = gateway.id().toString();

	Statement sql = (session() << m_queryRemove(),
		use(id, "id"),
		use(gatewayID, "gateway_id")
	);

	return execute(sql) > 0;
}

bool PocoSQLGatewayMessageDao::fetch(
		GatewayMessage &message,
		const Gateway &gateway)
{
	string id = message.id().toString();
	string gatewayID = gateway.id().toString();

	Statement sql = (session() << m_queryFetch(),
		use(id, "id"),
		use(gatewayID, "gateway_id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return false;

	return parseSingle(result, message);
}

void PocoSQLGatewayMessageDao::fetchBy(
		std::vector<GatewayMessage> &messages,
		const Gateway &gateway)
{
	string gatewayID = gateway.id().toString();

	Statement sql = (session() << m_queryFetchByGateway(),
		use(gatewayID, "gateway_id")
	);

	RecordSet result = executeSelect(sql);
	if (result.rowCount() == 0)
		return;

	parseMany(result, messages, gateway);
}

bool PocoSQLGatewayMessageDao::parseSingle(
		RecordSet &result,
		GatewayMessage &message,
		const string &prefix)
{
	if (result.begin() == result.end())
		return false;

	return parseSingle(*result.begin(), message, prefix);
}

bool PocoSQLGatewayMessageDao::parseSingle(
		Row &result,
		GatewayMessage &message,
		const string &prefix)
{
	if (hasColumn(result, prefix + "id"))
		message.setId(GatewayMessageID::parse(result[prefix + "id"]));

	message.setAt(Timestamp(result[prefix + "at"].convert<unsigned long>()));

	Gateway gateway;
	gateway.setId(GatewayID::parse(result[prefix + "gateway_id"]));
	message.setGateway(gateway);

	const int severity = result[prefix + "severity"];
	message.setSeverity(
		Severity::fromRaw(static_cast<Severity::Raw>(severity)));
	message.setKey(result[prefix + "key"]);

	const auto &context = result[prefix + "context"];
	if (!context.isEmpty())
		message.setContext(JsonUtil::parse(context));
	else
		message.setContext({});

	markLoaded(message);
	return true;
}
