#include "di/Injectable.h"
#include "dao/poco/PocoTransactionManager.h"
#include "dao/poco/PocoTransactionImpl.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, PocoTransactionManager)
BEEEON_OBJECT_CASTABLE(TransactionManager)
BEEEON_OBJECT_REF("daoManager", &PocoTransactionManager::setDaoManager)
BEEEON_OBJECT_END(BeeeOn, PocoTransactionManager)

PocoTransactionManager::PocoTransactionManager()
{
}

PocoTransactionManager::~PocoTransactionManager()
{
}

void PocoTransactionManager::setDaoManager(PocoDaoManager *manager)
{
	m_daoManager = manager;
}

void PocoTransactionManager::create(ThreadLocal<ThreadLocalTransaction *> &ref)
{
	new PocoTransaction(m_daoManager->pool(), ref);
}
