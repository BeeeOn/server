#ifndef BEEEON_OAUTH2_AUTH_PROVIDER_H
#define BEEEON_OAUTH2_AUTH_PROVIDER_H

#include "provider/AuthProvider.h"

namespace BeeeOn {

class OAuth2AuthProvider : public AuthCodeAuthProvider {
public:
	OAuth2AuthProvider(const std::string &name);

	void setClientId(const std::string &clientId)
	{
		m_clientId = clientId;
	}

	void setClientSecret(const std::string &clientSecret)
	{
		m_clientSecret = clientSecret;
	}

	void setRedirectURI(const std::string &uri)
	{
		m_redirectURI = uri;
	}

protected:
	/**
	 * Initialize SSL context for upcoming HTTPS requests
	 */
	void initSSL();

protected:
	std::string m_clientId;
	std::string m_clientSecret;
	std::string m_redirectURI;
};

}

#endif
