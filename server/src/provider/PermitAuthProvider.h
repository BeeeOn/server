#ifndef BEEEON_PERMIT_AUTH_PROVIDER_H
#define BEEEON_PERMIT_AUTH_PROVIDER_H

#include <Poco/Logger.h>
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

	bool verifyAuthCode(const std::string &authCode, Result &result)
	{
		m_logger.critical("PERMIT AUTH");
		result.insert(
			std::make_pair("email", authCode));
		return true;
	}
};

}

#endif
