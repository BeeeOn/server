#include <Poco/SingletonHolder.h>

#include "rpc/GatewayRPC.h"

BEEEON_OBJECT(BeeeOn_NullGatewayRPC, BeeeOn::NullGatewayRPC)

using namespace BeeeOn;

GatewayRPC &NullGatewayRPC::instance()
{
	static Poco::SingletonHolder<NullGatewayRPC> singleton;
	return *singleton.get();
}
