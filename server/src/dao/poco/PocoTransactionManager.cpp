#include "dao/poco/PocoTransactionManager.h"
#include "dao/poco/PocoTransactionImpl.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(BeeeOn, PocoTransactionManager)

PocoTransactionManager::PocoTransactionManager()
{
	injector("daoManager", &PocoTransactionManager::setDaoManager);
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
