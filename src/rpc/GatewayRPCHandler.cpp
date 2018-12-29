#include <Poco/Exception.h>

#include "rpc/GatewayRPCHandler.h"

using namespace Poco;
using namespace BeeeOn;

void GatewayRPCHandler::handle(GatewayRPCResult::Ptr result)
{
	switch (result->status()) {
	case GatewayRPCResult::Status::PENDING:
		onPending(result);
		break;

	case GatewayRPCResult::Status::ACCEPTED:
		onAccepted(result);
		break;

	case GatewayRPCResult::Status::SUCCESS:
	case GatewayRPCResult::Status::SUCCESS_PARTIAL:
		onSuccess(result);
		break;

	case GatewayRPCResult::Status::NOT_CONNECTED:
		onNotConnected(result);
		break;

	case GatewayRPCResult::Status::TIMEOUT:
		onTimeout(result);
		break;

	case GatewayRPCResult::Status::FAILED:
		onFailed(result);
		break;

	default:
		throw IllegalStateException("unexpected RPC result");
	}

	onAny(result);
}

void GatewayRPCHandler::onPending(GatewayRPCResult::Ptr result)
{
}

void GatewayRPCHandler::onAccepted(GatewayRPCResult::Ptr result)
{
}

void GatewayRPCHandler::onSuccess(GatewayRPCResult::Ptr result)
{
}

void GatewayRPCHandler::onNotConnected(GatewayRPCResult::Ptr result)
{
}

void GatewayRPCHandler::onTimeout(GatewayRPCResult::Ptr result)
{
}

void GatewayRPCHandler::onFailed(GatewayRPCResult::Ptr result)
{
}

void GatewayRPCHandler::onAny(GatewayRPCResult::Ptr result)
{
}
