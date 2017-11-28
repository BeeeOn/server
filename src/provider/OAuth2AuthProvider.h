#ifndef BEEEON_OAUTH2_AUTH_PROVIDER_H
#define BEEEON_OAUTH2_AUTH_PROVIDER_H

#include <Poco/SharedPtr.h>
#include <Poco/Net/Context.h>

#include "provider/AuthProvider.h"

namespace Poco {
class URI;
namespace Net {

class HTMLForm;
class HTTPSClientSession;

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

	void setSSLConfig(SSLClient *config)
	{
		m_sslConfig = config;
	}

protected:
	/**
	 * Verification against a 3rd party.
	 */
	virtual bool verifyAuthCode(
			const AuthCodeCredentials &credentials,
			AuthResult &info) = 0;

	/**
	 * Open HTTPS connection to the given host:port.
	 */
	Poco::SharedPtr<Poco::Net::HTTPSClientSession> connectSecure(
			const std::string &host,
			unsigned int port) const;

	/**
	 * Handle response based on given session.
	 */
	std::string handleResponse(Poco::Net::HTTPSClientSession &session) const;

	/**
	 * Convert istream response body to string
	 */
	std::string convertResponseToString(std::istream &rs) const;

	/**
	 * Creates session and makes on method based request and returns handled response
	 */
	std::string makeRequest(const std::string &method, Poco::URI &host, Poco::Net::HTMLForm &requestForm) const;

private:
	/**
	 * Initialize SSL context for upcoming HTTPS requests
	 */
	void initSSL() const;

protected:
	std::string m_clientId;
	std::string m_clientSecret;
	std::string m_redirectURI;
	SSLClient *m_sslConfig;
};

}

#endif
