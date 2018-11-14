#include "dao/AbstractGatewayDao.h"

using namespace std;
using namespace BeeeOn;

void AbstractGatewayDao::setCommunicator(GatewayCommunicator::Ptr communicator)
{
	m_communicator = communicator;
}

void AbstractGatewayDao::updateLastActivity(Gateway &gateway)
{
	const auto lastActivity = m_communicator->lastActivity(gateway.id());

	if (lastActivity.isNull())
		gateway.setLastActivity({});
	else
		gateway.setLastActivity({lastActivity.value()});
}

bool AbstractGatewayDao::insert(Gateway &gateway)
{
	const auto result = doInsert(gateway);
	updateLastActivity(gateway);
	return result;
}

bool AbstractGatewayDao::fetch(Gateway &gateway)
{
	const auto result = doFetch(gateway);
	updateLastActivity(gateway);
	return result;
}

bool AbstractGatewayDao::fetch(LegacyGateway &gateway, const User &user)
{
	const auto result = doFetch(gateway, user);
	updateLastActivity(gateway);
	return result;
}

bool AbstractGatewayDao::update(Gateway &gateway)
{
	const auto result = doUpdate(gateway);
	updateLastActivity(gateway);
	return result;
}

void AbstractGatewayDao::fetchAccessible(
	vector<Gateway> &gateways,
	const User &user)
{
	doFetchAccessible(gateways, user);

	for (auto &gateway : gateways)
		updateLastActivity(gateway);
}

void AbstractGatewayDao::fetchAccessible(
	vector<LegacyGateway> &gateways,
	const User &user)
{
	doFetchAccessible(gateways, user);

	for (auto &gateway : gateways)
		updateLastActivity(gateway);
}
