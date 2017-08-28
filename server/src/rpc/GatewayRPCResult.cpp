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

GatewayRPCResult::Status GatewayRPCResult::convert(const int value)
{
	switch (static_cast<Status>(value)) {
	case PENDING:
	case NOT_CONNECTED:
	case TIMEOUT:
	case ACCEPTED:
	case SUCCESS:
	case FAILED:
		return static_cast<Status>(value);
	default:
		throw InvalidArgumentException("invalid status: " + to_string(value));
	}
}
