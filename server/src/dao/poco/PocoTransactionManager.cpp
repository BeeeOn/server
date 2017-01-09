#include "dao/poco/PocoTransactionManager.h"
#include "dao/poco/PocoTransactionImpl.h"

using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT(PocoTransactionManager, BeeeOn::PocoTransactionManager)

PocoTransactionManager::PocoTransactionManager()
{
	injector<PocoTransactionManager, PocoDaoManager>(
		"daoManager",
		&PocoTransactionManager::setDaoManager
	);
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
