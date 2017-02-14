#include <Poco/SingletonHolder.h>

#include "rpc/GatewayRPC.h"

using namespace BeeeOn;

GatewayRPC &NullGatewayRPC::instance()
{
	static Poco::SingletonHolder<NullGatewayRPC> singleton;
	return *singleton.get();
}
