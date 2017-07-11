#ifndef BEEEON_POCO_SQL_GATEWAY_STATUS_DAO_H
#define BEEEON_POCO_SQL_GATEWAY_STATUS_DAO_H

#include "dao/GatewayStatusDao.h"
#include "dao/SQLQuery.h"
#include "dao/poco/PocoAbstractDao.h"

namespace BeeeOn {

class PocoSQLGatewayStatusDao :
		public PocoAbstractDao,
		public GatewayStatusDao {
public:
	PocoSQLGatewayStatusDao();

	bool insert(Gateway &gateway) override;

private:
	SQLQuery m_queryInsert {"gateways_status.insert"};
};

}

#endif
