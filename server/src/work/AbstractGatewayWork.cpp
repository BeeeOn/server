#include <string>

#include "work/AbstractGatewayWork.h"

using namespace std;
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
	json()->set("rpc_result", status.toString());
}

GatewayRPCResult::Status AbstractGatewayWork::result() const
{
	return GatewayRPCResult::Status::parse(
			json()->getValue<string>("rpc_result"));
}

bool AbstractGatewayWork::hasResult() const
{
	return json()->has("rpc_result");
}
