#include "rpc/GatewayRPC.h"

using namespace BeeeOn;

GatewayRPC::~GatewayRPC()
{
}

void GatewayRPC::doHandle(
		GatewayRPCHandler::Ptr handler,
		GatewayRPCResult::Ptr result)
{
	handler->handle(result);
}
