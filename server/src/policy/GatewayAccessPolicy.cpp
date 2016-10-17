#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "policy/GatewayAccessPolicy.h"

using namespace Poco;
using namespace BeeeOn;

void NullGatewayAccessPolicy::assureGet(
	const PolicyContext &context,
	const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullGatewayAccessPolicy::assureAssignGateway(
	const PolicyContext &context,
	const Place &place)
{
	throw NotImplementedException(__func__);
}

void NullGatewayAccessPolicy::assureUnassign(
	const PolicyContext &context,
	const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullGatewayAccessPolicy::assureUpdate(
	const PolicyContext &context,
	const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullGatewayAccessPolicy::assureScanDevices(
	const PolicyContext &context,
	const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

GatewayAccessPolicy &NullGatewayAccessPolicy::instance()
{
	static SingletonHolder<NullGatewayAccessPolicy> singleton;
	return *singleton.get();
}
