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
	GatewayID id(GatewayID::random());
	Gateway gateway(id);
	gateway.setName("Joe Doe's Gateway");

	m_gatewayDao->create(gateway);
}

void UIMockInit::injectionDone()
{
	initUsers();
	initGateways();
}
