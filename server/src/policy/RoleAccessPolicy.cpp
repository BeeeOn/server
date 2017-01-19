#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "policy/RoleAccessPolicy.h"

using namespace Poco;
using namespace BeeeOn;

NullRoleAccessPolicy::NullRoleAccessPolicy()
{
}

RoleAccessPolicy &NullRoleAccessPolicy::instance()
{
	static SingletonHolder<NullRoleAccessPolicy> singleton;
	return *singleton.get();
}

void NullRoleAccessPolicy::assureInvite(
		const PolicyContext &context,
		const Gateway &gateway,
		const AccessLevel &as)
{
	throw NotImplementedException(__func__);
}

void NullRoleAccessPolicy::assureList(
		const PolicyContext &context,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

void NullRoleAccessPolicy::assureRemove(
		const PolicyContext &context,
		const RoleInGateway &role)
{
	throw NotImplementedException(__func__);
}

void NullRoleAccessPolicy::assureUpdate(
		const PolicyContext &context,
		const RoleInGateway &role)
{
	throw NotImplementedException(__func__);
}
