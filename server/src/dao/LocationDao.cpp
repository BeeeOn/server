#include <Poco/SingletonHolder.h>

#include "dao/LocationDao.h"

BEEEON_OBJECT(NullLocationDao, BeeeOn::NullLocationDao)
BEEEON_OBJECT(MockLocationDao, BeeeOn::MockLocationDao)

using namespace BeeeOn;

LocationDao &NullLocationDao::instance()
{
	static Poco::SingletonHolder<NullLocationDao> singleton;
	return *singleton.get();
}

MockLocationDao::MockLocationDao():
	m_gatewayDao(NULL)
{
	injector<MockLocationDao, GatewayDao>("gatewayDao",
			&MockLocationDao::setGatewayDao);
}
