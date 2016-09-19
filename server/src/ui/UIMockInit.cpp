#include <Poco/UUIDGenerator.h>

#include "ui/UIMockInit.h"

BEEEON_OBJECT(UIMockInit, BeeeOn::UIMockInit)

using namespace Poco;
using namespace BeeeOn;

void UIMockInit::initUsers()
{
	UserID id(UserID::random());
	User user(id);
	user.setEmail("joe.doe@example.org");

	m_userDao->create(user);
}

void UIMockInit::initGateways()
{
	GatewayID id(GatewayID::parse("1284174504043136"));
	Gateway gateway(id);
	gateway.setName("Joe Doe's Gateway");

	m_gatewayDao->insert(gateway);

	GatewayID id2(GatewayID::parse("1780053541714013"));
	Gateway gateway2(id2);
	gateway2.setName("Gateway To Un/assign");

	m_gatewayDao->insert(gateway2);
}

void UIMockInit::injectionDone()
{
	initUsers();
	initGateways();
}
