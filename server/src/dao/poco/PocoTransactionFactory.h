#ifndef BEEEON_POCO_TRANSACTION_FACTORY_H
#define BEEEON_POCO_TRANSACTION_FACTORY_H

#include "dao/TransactionManager.h"
#include "dao/poco/PocoDaoManager.h"

namespace BeeeOn {

class PocoTransactionFactory : public TransactionFactory {
public:
	PocoTransactionFactory();
	~PocoTransactionFactory();

	void setDaoManager(PocoDaoManager *manager);

	Transaction *create() override;

private:
	PocoDaoManager *m_daoManager;
};

}

#endif
