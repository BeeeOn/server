#ifndef BEEEON_PERMIT_AUTH_PROVIDER_H
#define BEEEON_PERMIT_AUTH_PROVIDER_H

#include <Poco/Logger.h>
#include <Poco/String.h>
#include "provider/AuthProvider.h"

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
		logger().critical("SOME AUTHS WILL BE PERMITTED");
	}

	void setResultProvider(const std::string &provider)
	{
		m_resultProvider = provider;
	}

	bool verifyAuthCode(const std::string &authCode, AuthResult &result)
	{
		const std::string &email = Poco::trim(authCode);

		if (email.empty()) {
			logger().warning("given authCode is empty");
			return false;
		}

		logger().critical("PERMIT AUTH: " + authCode);
		result.setEmail(authCode);
		result.setProvider(m_resultProvider);
		return true;
	}

private:
	std::string m_resultProvider;
};

}

#endif
