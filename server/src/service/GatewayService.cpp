#include <Poco/Exception.h>

#include "service/GatewayService.h"

BEEEON_OBJECT(GatewayService, BeeeOn::GatewayService)

using namespace std;
using namespace Poco;
using namespace BeeeOn;

GatewayService::GatewayService():
	m_gatewayDao(&NullGatewayDao::instance()),
	m_rpc(&NullGatewayRPC::instance())
{
	injector<GatewayService, GatewayDao>("gatewayDao",
			&GatewayService::setGatewayDao);
	injector<GatewayService, GatewayRPC>("gatewayRPC",
			&GatewayService::setGatewayRPC);
}

bool GatewayService::fetch(Gateway &gateway)
{
	return m_gatewayDao->fetch(gateway);
}

bool GatewayService::fetchFromPlace(Gateway &gateway, const Place &place)
{
	return m_gatewayDao->fetchFromPlace(gateway, place);
}

void GatewayService::fetchAccessible(vector<Gateway> &gateways,
		const User &user)
{
	m_gatewayDao->fetchAccessible(gateways, user);
}

bool GatewayService::update(Gateway &gateway)
{
	return m_gatewayDao->update(gateway);
}

bool GatewayService::updateInPlace(Gateway &gateway,
		const Deserializer<Gateway> &update,
		const Place &place)
{
	if (!m_gatewayDao->fetchFromPlace(gateway, place))
		throw NotFoundException("gateway does not exist");

	update.partial(gateway);

	return m_gatewayDao->update(gateway);
}

bool GatewayService::assignAndUpdate(Gateway &gateway,
		const Deserializer<Gateway> &update,
		const Place &place)
{
	if (!m_gatewayDao->fetch(gateway))
		throw NotFoundException("gateway does not exist");

	if (gateway.hasPlace()) // do not leak it exists
		throw NotFoundException("gateway is already assigned");

	update.partial(gateway);

	return m_gatewayDao->assignAndUpdate(gateway, place);
}

bool GatewayService::unassign(Gateway &gateway, const Place &place)
{
	if (!m_gatewayDao->fetchFromPlace(gateway, place))
		return false;

	return m_gatewayDao->unassign(gateway);
}

void GatewayService::scanDevices(Gateway &gateway)
{
	m_rpc->sendListen(gateway);
}

void GatewayService::unpairDevice(Gateway &gateway, Device &device)
{
	m_rpc->unpairDevice(gateway, device);
}

void GatewayService::pingGateway(Gateway &gateway)
{
	m_rpc->pingGateway(gateway);
}
