#include <Poco/Exception.h>

#include "rpc/GatewayRPCHandler.h"

using namespace Poco;
using namespace BeeeOn;

void GatewayRPCHandler::handle(GatewayRPCResult::Ptr result)
{
	GatewayRPCHandler::Ptr self(this, true);

	self->onAny(result);
}

void GatewayRPCHandler::onAny(GatewayRPCResult::Ptr result)
{
}
