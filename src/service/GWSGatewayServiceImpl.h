#ifndef BEEEON_GWS_GATEWAY_SERVICE_IMPL_H
#define BEEEON_GWS_GATEWAY_SERVICE_IMPL_H

#include "dao/GatewayDao.h"
#include "dao/GatewayStatusDao.h"
#include "service/GWSGatewayService.h"
#include "transaction/Transactional.h"

namespace BeeeOn {

class GWSGatewayServiceImpl : public GWSGatewayService, public Transactional {
public:
	void setGatewayDao(GatewayDao::Ptr dao);
	void setGatewayStatusDao(GatewayStatusDao::Ptr dao);

	bool registerGateway(GatewayStatus &status, Gateway &gateway) override
	{
		return BEEEON_TRANSACTION_RETURN(bool, doRegisterGateway(status, gateway));
	}

protected:
	bool doRegisterGateway(GatewayStatus &status, Gateway &gateway);

private:
	GatewayDao::Ptr m_gatewayDao;
	GatewayStatusDao::Ptr m_gatewayStatusDao;
};

}

#endif
