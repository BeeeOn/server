#include <Poco/UUIDGenerator.h>

#include "ui/UIMockInit.h"

BEEEON_OBJECT(UIMockInit, BeeeOn::UIMockInit)

using namespace Poco;
using namespace BeeeOn;

void UIMockInit::injectionDone()
{
	UserID id(UserID::random());
	User user(id);
	user.setEmail("joe.doe@example.org");

	m_userDao->create(user);
}
