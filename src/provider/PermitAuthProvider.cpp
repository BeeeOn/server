#include <Poco/Logger.h>
#include <Poco/String.h>

#include "di/Injectable.h"
#include "provider/PermitAuthProvider.h"

BEEEON_OBJECT_BEGIN(BeeeOn, PermitAuthProvider)
BEEEON_OBJECT_CASTABLE(AuthProvider)
BEEEON_OBJECT_PROPERTY("resultProvider", &PermitAuthProvider::setResultProvider)
BEEEON_OBJECT_END(BeeeOn, PermitAuthProvider)

using namespace std;
using namespace BeeeOn;
using namespace Poco;

PermitAuthProvider::PermitAuthProvider():
	AbstractAuthProvider("permit")
{
	logger().critical("SOME AUTHS WILL BE PERMITTED");
}

void PermitAuthProvider::setResultProvider(const std::string &provider)
{
	m_resultProvider = provider;
}

bool PermitAuthProvider::authorize(const Credentials &cred, AuthResult &result)
{
	string email;

	if (typeid(cred) == typeid(AuthCodeCredentials)) {
		const AuthCodeCredentials &authCodeCredentials =
			static_cast<const AuthCodeCredentials &>(cred);

		email = trim(authCodeCredentials.authCode());
	}

	if (email.empty()) {
		logger().warning("extracted email is empty");
		return false;
	}

	logger().critical("PERMIT AUTH: " + email);
	result.setEmail(email);
	result.setProvider(m_resultProvider);
	return true;
}
