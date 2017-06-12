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

class OAuth2AuthProvider : public AuthCodeAuthProvider {
public:
	OAuth2AuthProvider(const std::string &name);

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

	void setSSLConfig(SSLClient *config)
	{
		m_sslConfig = config;
	}

protected:
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
