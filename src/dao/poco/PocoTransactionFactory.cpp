#include "di/Injectable.h"
#include "dao/poco/PocoTransactionFactory.h"
#include "dao/poco/PocoTransactionImpl.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoTransactionFactory)
BEEEON_OBJECT_CASTABLE(TransactionFactory)
BEEEON_OBJECT_PROPERTY("daoManager", &PocoTransactionFactory::setDaoManager)
BEEEON_OBJECT_END(BeeeOn, PocoTransactionFactory)

PocoTransactionFactory::PocoTransactionFactory()
{
}

PocoTransactionFactory::~PocoTransactionFactory()
{
}

void PocoTransactionFactory::setDaoManager(PocoDaoManager *manager)
{
	m_daoManager = manager;
}

Transaction *PocoTransactionFactory::create()
{
	return new PocoTransaction(m_daoManager->pool());
}
