#ifndef BEEEON_POCO_SQL_GATEWAY_STATUS_DAO_H
#define BEEEON_POCO_SQL_GATEWAY_STATUS_DAO_H

#include "dao/GatewayStatusDao.h"
#include "dao/Query.h"
#include "dao/poco/PocoAbstractDao.h"

namespace BeeeOn {

class PocoSQLGatewayStatusDao :
		public PocoAbstractDao,
		public GatewayStatusDao {
public:
	PocoSQLGatewayStatusDao();

	bool insert(const GatewayStatus &status, const Gateway &gateway) override;

private:
	Query m_queryInsert {"gateways_status.insert"};
};

}

#endif
