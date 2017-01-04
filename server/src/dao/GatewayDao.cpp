#include <Poco/SingletonHolder.h>

#include "dao/GatewayDao.h"

BEEEON_OBJECT(NullGatewayDao, BeeeOn::NullGatewayDao)

using namespace std;
using namespace BeeeOn;

GatewayDao &NullGatewayDao::instance()
{
	static Poco::SingletonHolder<NullGatewayDao> singleton;
	return *singleton.get();
}
