#include <Poco/Data/Statement.h>

#include "di/Injectable.h"
#include "dao/poco/PocoSQLGatewayStatusDao.h"
#include "dao/poco/PocoDaoManager.h"
#include "transaction/TransactionManager.h"

using namespace std;
using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLGatewayStatusDao)
BEEEON_OBJECT_CASTABLE(GatewayStatusDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLGatewayStatusDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLGatewayStatusDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLGatewayStatusDao::setSQLLoader)
BEEEON_OBJECT_HOOK("done", &PocoSQLGatewayStatusDao::loadQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLGatewayStatusDao)

PocoSQLGatewayStatusDao::PocoSQLGatewayStatusDao()
{
	registerQuery(m_queryInsert);
}

bool PocoSQLGatewayStatusDao::insert(Gateway &gateway)
{
	assureHasId(gateway);

	string id(gateway.id().toString());

	Nullable<unsigned long> at;
	if (!gateway.lastChanged().isNull())
		at = gateway.lastChanged().value().timestamp().epochTime();

	Nullable<string> version;
	if (!gateway.version().empty())
		version = gateway.version();

	Nullable<string> ip;
	if (!gateway.ipAddress().isWildcard())
		ip = gateway.ipAddress().toString();

	Statement sql = (session() << m_queryInsert(),
		use(id, "gateway_id"),
		use(at, "at"),
		use(version, "version"),
		use(ip, "ip")
	);

	return execute(sql) > 0;
}
