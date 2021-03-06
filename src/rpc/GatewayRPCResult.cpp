#include <string>

#include <Poco/Exception.h>

#include "rpc/GatewayRPCResult.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GatewayRPCResult::GatewayRPCResult():
	m_status(Status::PENDING)
{
}

GatewayRPCResult::~GatewayRPCResult()
{
}

void GatewayRPCResult::setStatus(const Status status)
{
	m_status = status;
}

GatewayRPCResult::Status GatewayRPCResult::status() const
{
	return m_status;
}

EnumHelper<GatewayRPCResult::StatusEnum::Raw>::ValueMap
	&GatewayRPCResult::StatusEnum::valueMap()
{
	static EnumHelper<GatewayRPCResult::StatusEnum::Raw>::ValueMap valueMap = {
		{GatewayRPCResult::StatusEnum::PENDING, "pending"},
		{GatewayRPCResult::StatusEnum::NOT_CONNECTED, "not_connected"},
		{GatewayRPCResult::StatusEnum::TIMEOUT, "timeout"},
		{GatewayRPCResult::StatusEnum::ACCEPTED, "accepted"},
		{GatewayRPCResult::StatusEnum::SUCCESS, "success"},
		{GatewayRPCResult::StatusEnum::SUCCESS_PARTIAL, "success_partial"},
		{GatewayRPCResult::StatusEnum::FAILED, "failed"},
	};

	return valueMap;
}
