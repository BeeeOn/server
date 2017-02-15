#ifndef BEEEON_POCO_TRANSACTION_MANAGER_H
#define BEEEON_POCO_TRANSACTION_MANAGER_H

#include "dao/ThreadLocalTransactionManager.h"
#include "dao/poco/PocoDaoManager.h"

namespace BeeeOn {

class PocoTransactionManager : public ThreadLocalTransactionManager {
public:
	PocoTransactionManager();
	~PocoTransactionManager();

	void setDaoManager(PocoDaoManager *manager);

protected:
	void create(Poco::ThreadLocal<ThreadLocalTransaction *> &ref);

private:
	PocoDaoManager *m_daoManager;
};

}

#endif
