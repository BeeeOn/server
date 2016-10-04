#include <Poco/UUIDGenerator.h>

#include "ui/UIMockInit.h"

BEEEON_OBJECT(UIMockInit, BeeeOn::UIMockInit)

using namespace Poco;
using namespace BeeeOn;

void UIMockInit::initUsers()
{
	User joeDoeUser(UserID::random());
	joeDoeUser.setFirstName("Joe");
	joeDoeUser.setLastName("Doe");

	m_userDao->create(joeDoeUser);

	Identity joeDoe0;
	joeDoe0.setEmail("joe.doe@example.org");
	joeDoe0.setUser(joeDoeUser);

	m_identityDao->create(joeDoe0);
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
