#ifndef BEEEON_PERMIT_AUTH_PROVIDER_H
#define BEEEON_PERMIT_AUTH_PROVIDER_H

#include "provider/AuthProvider.h"

namespace BeeeOn {

/**
 * This authorization provider permits all users
 * to access the system. It is useful for testing
 * purposes. When it is activated a loud logs are
 * created on start and on every login.
 */
class PermitAuthProvider : public AbstractAuthProvider {
public:
	PermitAuthProvider();

	void setResultProvider(const std::string &provider);

	bool authorize(const Credentials &cred, AuthResult &result) override;

private:
	std::string m_resultProvider;
};

}

#endif
