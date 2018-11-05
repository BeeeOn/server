#include "dao/AbstractGatewayDao.h"

using namespace std;
using namespace BeeeOn;

bool AbstractGatewayDao::insert(Gateway &gateway)
{
	const auto result = doInsert(gateway);
	return result;
}

bool AbstractGatewayDao::fetch(Gateway &gateway)
{
	const auto result = doFetch(gateway);
	return result;
}

bool AbstractGatewayDao::fetch(LegacyGateway &gateway, const User &user)
{
	const auto result = doFetch(gateway, user);
	return result;
}

bool AbstractGatewayDao::update(Gateway &gateway)
{
	const auto result = doUpdate(gateway);
	return result;
}

void AbstractGatewayDao::fetchAccessible(
	vector<Gateway> &gateways,
	const User &user)
{
	doFetchAccessible(gateways, user);
}

void AbstractGatewayDao::fetchAccessible(
	vector<LegacyGateway> &gateways,
	const User &user)
{
	doFetchAccessible(gateways, user);
}
