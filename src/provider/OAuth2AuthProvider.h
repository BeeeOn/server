#ifndef BEEEON_OAUTH2_AUTH_PROVIDER_H
#define BEEEON_OAUTH2_AUTH_PROVIDER_H

#include <Poco/SharedPtr.h>
#include <Poco/Net/Context.h>

#include "net/HTTPEntireResponse.h"
#include "provider/AuthProvider.h"

namespace Poco {
class URI;
namespace Net {

class HTMLForm;

}
}

namespace BeeeOn {

class SSLClient;

/**
 * An OAuth 2 authorization provider performs authorization by using an
 * auth code. The abstract class extracts the appropriate credentials and
 * verifies it against a 3rd party authorization service.
 */
class OAuth2AuthProvider : public AbstractAuthProvider {
public:
	OAuth2AuthProvider(const std::string &name);

	bool authorize(const Credentials &cred, AuthResult &result) override;

	void setClientId(const std::string &clientId)
	{
		m_clientId = clientId;
	}

	std::string clientId() const
	{
		return m_clientId;
	}

	void setClientSecret(const std::string &clientSecret)
	{
		m_clientSecret = clientSecret;
	}

	void setRedirectURI(const std::string &uri)
	{
		m_redirectURI = uri;
	}

	std::string redirectURI() const
	{
		return m_redirectURI;
	}

	void setSSLConfig(Poco::SharedPtr<SSLClient> config)
	{
		m_sslConfig = config;
	}

	/**
	 * Initialize SSL context for upcoming HTTPS requests
	 */
	void validateSSL();

protected:
	/**
	 * Verification against a 3rd party.
	 */
	virtual bool verifyAuthCode(
			const AuthCodeCredentials &credentials,
			AuthResult &info);
	/**
	 * Verification against a 3rd party.
	 */
	virtual bool verifyAuthCode(
			const AccessTokenCredentials &credentials,
			AuthResult &info);

	/**
	 * Handle response.
	 */
	std::string handleResponse(HTTPEntireResponse &session) const;

	/**
	 * Creates session and makes on method based request and returns handled response
	 */
	std::string makeRequest(const std::string &method, Poco::URI &host, Poco::Net::HTMLForm &requestForm) const;

protected:
	std::string m_clientId;
	std::string m_clientSecret;
	std::string m_redirectURI;
	Poco::SharedPtr<SSLClient> m_sslConfig;
};

}

#endif
