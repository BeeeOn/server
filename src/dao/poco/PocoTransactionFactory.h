#pragma once

#include "dao/poco/PocoDaoManager.h"
#include "transaction/TransactionManager.h"

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
