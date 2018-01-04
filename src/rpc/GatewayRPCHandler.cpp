#include <Poco/Exception.h>

#include "rpc/GatewayRPCHandler.h"

using namespace Poco;
using namespace BeeeOn;

std::function<void(GatewayRPCResult::Ptr)> GatewayRPCHandler::lambda()
{
	GatewayRPCHandler::Ptr self(this, true);

	return [=](GatewayRPCResult::Ptr r) mutable {
		self->onAny(r);
	};
}

void GatewayRPCHandler::onAny(GatewayRPCResult::Ptr result)
{
}
