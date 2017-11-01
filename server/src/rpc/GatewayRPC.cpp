#include <Poco/SingletonHolder.h>

#include "rpc/GatewayRPC.h"

using namespace BeeeOn;

GatewayRPC::~GatewayRPC()
{
}

GatewayRPC &NullGatewayRPC::instance()
{
	static Poco::SingletonHolder<NullGatewayRPC> singleton;
	return *singleton.get();
}
