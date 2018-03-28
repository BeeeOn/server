#include "di/Injectable.h"
#include "service/GWSGatewayServiceImpl.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

BEEEON_OBJECT_BEGIN(BeeeOn, GWSGatewayServiceImpl)
BEEEON_OBJECT_CASTABLE(GWSGatewayService)
BEEEON_OBJECT_PROPERTY("gatewayDao", &GWSGatewayServiceImpl::setGatewayDao)
BEEEON_OBJECT_PROPERTY("gatewayStatusDao", &GWSGatewayServiceImpl::setGatewayStatusDao)
BEEEON_OBJECT_PROPERTY("transactionManager", &Transactional::setTransactionManager)
BEEEON_OBJECT_END(BeeeOn, GWSGatewayServiceImpl)

void GWSGatewayServiceImpl::setGatewayDao(GatewayDao::Ptr dao)
{
	m_gatewayDao = dao;
}

void GWSGatewayServiceImpl::setGatewayStatusDao(GatewayStatusDao::Ptr dao)
{
	m_gatewayStatusDao = dao;
}

bool GWSGatewayServiceImpl::doRegisterGateway(GatewayStatus &status,
		Gateway &gateway)
{
	status.setLastChanged(DateTime());

	if (m_gatewayDao->fetch(gateway)) {
		m_gatewayStatusDao->insert(status, gateway);
		return true;
	}

	if (!m_gatewayDao->insert(gateway))
		return false;

	return m_gatewayStatusDao->insert(status, gateway);
}
