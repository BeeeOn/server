#include <Poco/Exception.h>
#include <Poco/SingletonHolder.h>

#include "dao/RoleInGatewayDao.h"

using namespace std;
using namespace Poco;
using namespace BeeeOn;

NullRoleInGatewayDao::NullRoleInGatewayDao()
{
}

void NullRoleInGatewayDao::create(RoleInGateway &role)
{
	throw NotImplementedException(__func__);
}

void NullRoleInGatewayDao::fetchBy(vector<RoleInGateway> &roles,
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

bool NullRoleInGatewayDao::remove(const RoleInGateway &role)
{
	throw NotImplementedException(__func__);
}

bool NullRoleInGatewayDao::remove(
		const Gateway &gateway, const User &user)
{
	throw NotImplementedException(__func__);
}

bool NullRoleInGatewayDao::isRegistered(
		const Gateway &gateway)
{
	throw NotImplementedException(__func__);
}

AccessLevel NullRoleInGatewayDao::fetchAccessLevel(
	const Gateway &gateway,
	const User &user)
{
	throw NotImplementedException(__func__);
}


void NullRoleInGatewayDao::fetchAccessibleGateways(
	vector<Gateway> &list,
	const User &user,
	const AccessLevel &atLeast)
{
	throw NotImplementedException(__func__);
}


RoleInGatewayDao &NullRoleInGatewayDao::instance()
{
	static SingletonHolder<NullRoleInGatewayDao> singleton;
	return *singleton.get();
}
