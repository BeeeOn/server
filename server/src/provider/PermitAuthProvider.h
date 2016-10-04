#ifndef BEEEON_PERMIT_AUTH_PROVIDER_H
#define BEEEON_PERMIT_AUTH_PROVIDER_H

#include <Poco/Logger.h>
#include <Poco/String.h>
#include "provider/AuthProvider.h"
#include "Debug.h"

namespace BeeeOn {

/**
 * This authorization provider permits all users
 * to access the system. It is useful for testing
 * purposes. When it is activated a loud logs are
 * created on start and on every login.
 */
class PermitAuthProvider : public AuthCodeAuthProvider {
public:
	PermitAuthProvider():
		AuthCodeAuthProvider("permit")
	{
		m_logger.critical("SOME AUTHS WILL BE PERMITTED");
	}

	bool verifyAuthCode(const std::string &authCode, AuthResult &result)
	{
		const std::string &email = Poco::trim(authCode);

		if (email.empty()) {
			m_logger.warning("given authCode is empty");
			return false;
		}

		m_logger.critical("PERMIT AUTH: " + authCode);
		result.setEmail(authCode);
		return true;
	}
};

}

#endif
