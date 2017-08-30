#include "work/AbstractGatewayWork.h"

using namespace Poco::JSON;
using namespace BeeeOn;

AbstractGatewayWork::AbstractGatewayWork(const WorkContent &copy):
	WorkContent(copy)
{
}

AbstractGatewayWork::AbstractGatewayWork(const ClassInfo &type):
	WorkContent(type)
{
}

AbstractGatewayWork::AbstractGatewayWork(Object::Ptr content):
	WorkContent(content)
{
}

void AbstractGatewayWork::setResult(const GatewayRPCResult::Status status)
{
	json()->set("rpc_result", static_cast<int>(status));
}

GatewayRPCResult::Status AbstractGatewayWork::result() const
{
	return GatewayRPCResult::convert(json()->getValue<int>("rpc_result"));
}

bool AbstractGatewayWork::hasResult() const
{
	return json()->has("rpc_result");
}
