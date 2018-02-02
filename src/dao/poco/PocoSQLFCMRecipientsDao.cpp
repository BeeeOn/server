#include <Poco/Data/Session.h>
#include <Poco/Data/Statement.h>
#include <Poco/Data/RecordSet.h>
#include <Poco/Data/Row.h>
#include <Poco/Data/RowIterator.h>

#include "di/Injectable.h"
#include "transaction/TransactionManager.h"

#include "dao/poco/PocoDaoManager.h"
#include "dao/poco/PocoSQLFCMRecipientsDao.h"
#include "dao/poco/PocoSQLFCMTokenDao.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PocoSQLFCMRecipientsDao)
BEEEON_OBJECT_CASTABLE(FCMRecipientsDao)
BEEEON_OBJECT_REF("daoManager", &PocoSQLFCMRecipientsDao::setDaoManager)
BEEEON_OBJECT_REF("transactionManager", &PocoSQLFCMRecipientsDao::setTransactionManager)
BEEEON_OBJECT_REF("sqlLoader", &PocoSQLFCMRecipientsDao::setQueryLoader)
BEEEON_OBJECT_REF("profiler", &PocoSQLFCMRecipientsDao::setQueryProfiler)
BEEEON_OBJECT_HOOK("done", &PocoSQLFCMRecipientsDao::loadQueries)
BEEEON_OBJECT_HOOK("cleanup", &PocoSQLFCMRecipientsDao::clearQueries)
BEEEON_OBJECT_END(BeeeOn, PocoSQLFCMRecipientsDao)

using namespace Poco;
using namespace Poco::Data;
using namespace Poco::Data::Keywords;
using namespace BeeeOn;

PocoSQLFCMRecipientsDao::PocoSQLFCMRecipientsDao()
{
	registerQuery(m_queryByGateway);
}

void PocoSQLFCMRecipientsDao::fetchBy(
		std::vector<FCMToken> &tokens,
		const Gateway &gateway)
{
	assureHasId(gateway);

	uint64_t gatewayID(gateway.id());

	Statement sql = (session() << m_queryByGateway(),
			use(gatewayID, "gateway_id")
	);

	RecordSet result = executeSelect(sql);
	PocoSQLFCMTokenDao::parseMany(result, tokens);
}
