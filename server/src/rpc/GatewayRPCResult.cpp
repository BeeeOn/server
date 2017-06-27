#include "rpc/GatewayRPCResult.h"

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

